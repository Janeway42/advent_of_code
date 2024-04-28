#include <pthread.h>
#include "../basics/basics.h"

#define BUFFER 1024
#define DATA 3

typedef struct s_data
{
	int count;
	pthread_t *thread;
	char *file;
	double start;
	double end;
	double output;
	bool done;
	bool error;
	struct s_monitor *surveillance;
}      			t_data;

typedef struct s_monitor
{
	pthread_t *thread;
	pthread_mutex_t	lock_surveillance;
	t_data **storage;
	int size;
}      			t_monitor;

//--------------------------------------------------

void free_storage(t_data ***storage, int location)
{
 	for (int i = 0; i < location; i++)
	{
		free((*storage)[i]->file);
		free((*storage)[i]->thread);
		free((*storage)[i]);
	}
	free(*storage);
}

int get_seeds(char *file_name)
{
	FILE *file = fopen(file_name, "r");
	if (file == NULL)
		basics_exit_error("no file! go away!", 1);
	
	char buffer[BUFFER];
	char *line;
	line = fgets(buffer, BUFFER, file);
	int size = 0;
	char *temp = strstr(line, " ");
	for (int i = 0; temp[i] != '\0'; i++)
	{
		if (isdigit(temp[i]) != 0 && (temp[i + 1] == '\n' || temp[i + 1] == ' '))
			size++;
	}
	fclose(file);
	return (size);
}

double make_double(char *str)
{
	double number;
	int size = strlen(str) - 1;
	if (str[size] == '\n')
		size--;
	number = str[size] - '0';

	double multiplier = 10;
	for (int i = size - 1; i > -1 && isdigit(str[i]) != 0; i--)
	{
		number = ((str[i] - '0') * multiplier) + number;
		multiplier *= 10;
	}
	return (number);
}

static void	*routine(void *arg)
{
	t_data *seed;
	seed = (t_data *)arg;

	printf("start thread %d\n", seed->count);

	FILE *file = fopen(seed->file, "r");
	if (file == NULL)
	{
		pthread_mutex_lock(&seed->surveillance->lock_surveillance);
		seed->error = true;
		pthread_mutex_unlock(&seed->surveillance->lock_surveillance);
		printf("no file! go away! thread %d! Sending cancellation request!\n", seed->count);
		return (NULL);
	}

	char buffer[BUFFER];
	char *line;

	for (int i = 0; i < seed->end; i++)
	{
		line = fgets(buffer, BUFFER, file);
		double backup = seed->start;

		while (line = fgets(buffer, BUFFER, file))
		{
			int tracker = 0;
			while (line[0] != '\n')
			{
				if (isdigit(line[0]) == 0)
					line = fgets(buffer, BUFFER, file);

				if (tracker == 0)
				{
					double array[DATA];
					char *temp;

					for (int i = 0; (temp = strsep(&line,  " ")) != NULL || (temp = strsep(&line, "\n")) != NULL; i++)
						array[i] = basics_string_to_double(temp);
					if (seed->start >= array[1] && seed->start < (array[1] + array[2]))
					{
						seed->start = array[0] + (seed->start - array[1]);
						tracker = 1;
					}
				}
				line = fgets(buffer, BUFFER, file);
				if(!line)
					break ;
			}
		}
		if (seed->output == -1)
			seed->output = seed->start;
		else if (seed->output > seed->start)
			seed->output = seed->start;
		seed->start = backup;
		seed->start++;
		rewind(file);
	}
	fclose(file);
	pthread_mutex_lock(&seed->surveillance->lock_surveillance);
	seed->done = true;
	pthread_mutex_unlock(&seed->surveillance->lock_surveillance);
	printf("thread %d done-------------- output: %0.f\n", seed->count, seed->output);
	return (NULL);
}

static bool get_done(t_monitor *data, int i)
{
	bool out;

	pthread_mutex_lock(&data->lock_surveillance);
	out = data->storage[i]->done;
	pthread_mutex_unlock(&data->lock_surveillance);
	return (out);
}

static bool get_error(t_monitor *data, int i)
{
	bool out;

	pthread_mutex_lock(&data->lock_surveillance);
	out = data->storage[i]->error;
	pthread_mutex_unlock(&data->lock_surveillance);
	return (out);
}

static void *surveillance(void *arg)
{
	t_monitor *data;
	data = (t_monitor *)arg;

	printf("start surveilance!\n");

	while (1)
	{
		bool done = true;
		bool error_thread = false;
		for (int i = 0; i < data->size; i++)
		{
			error_thread = get_error(data, i);
			if (error_thread == true)
			{
				for (int i = 0; i < data->size; i++)
				{
					int er = pthread_cancel((*(data->storage[i]->thread)));
					printf("thread cancel for thread: %d = %d\n", i, er);
				}
				return (NULL);
			}
		}
		for (int i = 0; i < data->size; i++)
		{
			done = get_done(data, i);
			if (done == false)
				break ;
		} 
		if (done == true)
			break ;
		sleep(240);
	}
	return (NULL);
}

static int	start_threads(t_data **storage, t_monitor *surveil_data, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (pthread_create(storage[i]->thread, NULL,
				&routine, storage[i]) != 0)
			{
				free_storage(&storage, size);
				free(surveil_data->thread);
				free(surveil_data);
				return (basics_exit_error("failed threads", KO));
			}
	}
	if (pthread_create(surveil_data->thread, NULL,
				&surveillance, surveil_data) != 0)
			{
				free_storage(&storage, size);
				free(surveil_data->thread);
				free(surveil_data);
				return (basics_exit_error("failed threads", KO));
			}
	return (OK);
}

static int	fill_storage(t_data ***storage, t_monitor *surveil_data, char *file_name, int size)
{
	FILE *file = fopen(file_name, "r");
	if (file == NULL)
		basics_exit_error("no file! go away!", KO);

	char buffer[BUFFER];
	char *line;
	line = fgets(buffer, BUFFER, file);
	fclose(file);

	line = strstr(line, " ");
	char *dup = strdup(line);
	char *backup;
	backup = dup;
	dup++;

	char *temp;
	for (int i = 0; i < size; i++)
	{
		(*storage)[i]->count = i;
		(*storage)[i]->file = strdup(file_name);
		(*storage)[i]->thread = malloc(sizeof(pthread_t));
		if (!(*storage)[i]->thread)
		{
			free_storage(storage, i);
			basics_exit_error("malloc failure", MFAIL);
		}
		if ((temp = strsep(&dup, " ")) != NULL || (temp = strsep(&dup, "\n")))
			(*storage)[i]->start = make_double(temp);
		if ((temp = strsep(&dup, " ")) != NULL || (temp = strsep(&dup, "\n")))
			(*storage)[i]->end = make_double(temp);
		(*storage)[i]->output = -1;
		(*storage)[i]->done = false;
		(*storage)[i]->error = false;
		(*storage)[i]->surveillance = surveil_data;
	}
	free(backup);
	return (OK);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (KO);

	printf("\nDepending on your computer resources this  w i l l  take a while...\n");
	printf("          You've been warned! Enter at your own peril!\n\n");

	int size = get_seeds(argv[1]) / 2;

	// create the storage structure 
	t_data **storage;
	storage = malloc(sizeof(t_data *) * size);
	if(!storage)
		basics_exit_error("malloc failure", MFAIL);
	for (int i; i < size; i++)
	{
		storage[i] = malloc(sizeof(t_data));
		if (!storage[i])
		{
			for (i; i > -1; i--)
				free(storage[i]);
			free(storage);
			basics_exit_error("malloc failure", MFAIL);
		}
	}

	// create the surveilance structure 
	t_monitor *surveil_data;
	surveil_data = malloc(sizeof(t_monitor));
	if(!surveil_data)
		basics_exit_error("malloc failure", MFAIL); 
	surveil_data->storage = storage;
	surveil_data->thread = malloc(sizeof(pthread_t));
	if (!surveil_data->thread)
		basics_exit_error("malloc failure", MFAIL);
	if (pthread_mutex_init(&surveil_data->lock_surveillance, NULL) != 0)
	{
		free_storage(&storage, size);
		basics_exit_error("mutexes failure", KO);
	}
	surveil_data->size = size;

	fill_storage(&storage, surveil_data, argv[1], size);
	start_threads(storage, surveil_data, size);

	// join threads 
	for (int i = 0; i < size; i++)
		pthread_join((*(storage[i]->thread)), NULL);
	pthread_join((*(*surveil_data).thread), NULL);

	// calculate smallest 
	double smallest = storage[0]->output;
	for (int i = 0; i < size; i++)
	{
		if (smallest > storage[i]->output)
			smallest = storage[i]->output;
	}

	// cleanup
	free_storage(&storage, size);
	pthread_mutex_destroy(&surveil_data->lock_surveillance);
	free(surveil_data->thread);
	free(surveil_data);

	if (smallest == -1)
		(printf("threads cancelled! ouput failed!\n"));
	else
		printf("day 5 part 2: %0.f\n", smallest);
	return (OK);
}