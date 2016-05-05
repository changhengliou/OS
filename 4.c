#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

void *sort(void* array);
void *sort2(void* array);
void *merge(void* array);

int arr[] = {17, 72, 93, 23, 45, 31 ,88, 60};
int size = sizeof(arr)/sizeof(int);

/*Sort 1 for first thread*/
void *sort(void* array){
	int i = 0, j = 0, temp = 0;

	printf("Thread 1 Before Sort:\n");
	for(;i<size/2;i++){
		printf("%d ", *((int*)array + i));
	}
	printf("\n");

	for(i=0;i<size/2;i++){
		for(j=i+1;j<size/2;j++){
			if(*((int*)array+i) > *((int*)array+j)){
				temp = *((int*)array+i);
				*((int*)array+i) = *((int*)array+j);
				*((int*)array+j) = temp;
			}
		}
	}
	printf("Thread 1 After Sort:\n");
        for(i=0;i<size/2;i++){
                printf("%d ", *((int*)array + i));
        }
        printf("\n");

	pthread_exit(NULL);
}

void *sort2(void* array){
        int i = 0, j = 0, temp = 0;

	printf("Thread 2 Before Sort:\n");
	for(i = size/2 ;i<size;i++){
                printf("%d ", *((int*)array + i));
        }
        printf("\n");

        for(i=size/2;i<size;i++){
                for(j=i+1;j<size;j++){
                        if(*((int*)array+i) > *((int*)array+j)){
                                temp = *((int*)array+i);
                                *((int*)array+i) = *((int*)array+j);
                                *((int*)array+j) = temp;
                        }
                }
        }
	printf("Thread 2 After Sort:\n");
        for(i=size/2;i<size;i++){
                printf("%d ", *((int*)array + i));
        }
        printf("\n");

        pthread_exit(NULL);
}

void* merge(void* array){
        int i = 0, j = 0, temp = 0;
        printf("Merge Start:\n");
        for(i = 0;i<size;i++){
                printf("%d ", *((int*)array + i));
        }
        printf("\n");
        for(i=0;i<size;i++){
                for(j=i+1;j<size;j++){
                        if(*((int*)array+i) > *((int*)array+j)){
                                temp = *((int*)array+i);
                                *((int*)array+i) = *((int*)array+j);
                                *((int*)array+j) = temp;
                        }
                }
        }
        printf("After Merge:\n");
        for(i=0;i<size;i++){
                printf("%d ", *((int*)array + i));
        }
        printf("\n");
        pthread_exit(NULL);
}

int main(){
    pthread_t sort_T1, sort_T2, merge_T;
    int i = 0;

    printf("Before Sort:\n");
        for(i=0;i<size;i++){
                printf("%d ", arr[i]);
        }
    printf("\n");
    pthread_create(&sort_T1, NULL , sort, (void*) arr);  //create(*tid, *attr, *function, *arg)
    pthread_create(&sort_T2, NULL , sort2, (void*) arr);

    pthread_join(sort_T1, NULL); //(pthread, **ptr)  stop  current thread until thread finish
    pthread_join(sort_T2, NULL);

    pthread_create(&merge_T, NULL , merge, (void*) arr);
    pthread_join(merge_T, NULL);

    return 0;
}
