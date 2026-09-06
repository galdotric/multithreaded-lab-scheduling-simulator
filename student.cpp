#include <vector>
#include <pthread.h>
using namespace std;

struct Student{
	int studentID;
	pthread_t thread;
	int groupID;
};


void initializeStudent(Student& student, int studentID){
	student.studentID = studentID;
	student.groupID = -1;
}

void* StudentRoutine(void* arg){
	return NULL;
}


vector<Student> createStudent(int studentNumber){
	vector<Student> Class(studentNumber);

	for(int id = 0; id < studentNumber; id++){
		initializeStudent(Class[id], id);
		pthread_create(&Class[id].thread, NULL, StudentRoutine, NULL);
	}

	return Class;
}


