#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Course 
{
    char course_name[50];
    int sem;
    char dept[3];
    int num_classes;
    struct Course *next;
} Course;

typedef struct fee
 {
    char rno[11];
    char status[8];
    struct fee* next;
} Fee;


typedef struct applicant 
{
    char name[100];
    char rno[11];
    char status[3];
    struct applicant* next;
} Applicant;

typedef struct AttendanceRecordList
 {
    char rno[11];
    char dept[3];
    int numOfClasses;
    float percentageAttendance;
    struct AttendanceRecordList* next;
} Attendance;

typedef struct student 
{
    char name[100];
    char dept[3];
    int sem;
    char rno[11];
    int classes_conducted[5];
    int classes_attended[5];
    struct student* next;
    Applicant* this_application;
    Fee* this_fee;
    Attendance* this_attendance;
}Student;

typedef struct Studentrec
{
    char name[100];
    char dept[3];
    int sem;
    char rno[11];
    struct Studentrec* next;
}StudentList;

//Q3
StudentList* CreatListOfEligibleStudents(Student* last  ){
    Student* current = last->next;
    StudentList* list = NULL;
    StudentList* list1 = list;
    do{
        if(strcmp(current->this_fee->status,"clear")==0 && current->this_attendance->percentageAttendance>75 && strcmp(current->this_application->status,"A")==0){
            // copying the details of this student into list of eligible student.
            if(list==NULL){
                list = (StudentList*) malloc(sizeof(StudentList));
                list->sem = current->sem;
                strcpy(list->dept , current->dept);
                strcpy(list->name , current->name);
                strcpy(list->rno , current->rno);
                list->next=NULL;
                list1=list;
            }else{
                list->next = (StudentList*) malloc(sizeof(StudentList));
                list = list->next;
                list->sem = current->sem;
                strcpy(list->dept , current->dept);
                strcpy(list->name , current->name);
                strcpy(list->rno , current->rno);
                list->next=NULL;
            }
        }
        current=current->next;
    }while(current!=last->next);
    return list1;
}

//Q4
StudentList* lessThanOrEqTo75(Student* last  ){
    Student* current = last->next;
    StudentList* list = NULL;
    StudentList* list1 = list;
    do{
        if(current->this_attendance->percentageAttendance<=75.0000f){
            // copying the details of this student into list of student having less tham or equal to 75%.
            if(list==NULL){
                list = (StudentList*) malloc(sizeof(StudentList));
                list->sem = current->sem;
                strcpy(list->dept , current->dept);
                strcpy(list->name , current->name);
                strcpy(list->rno , current->rno);
                list->next=NULL;
                list1=list;
            }else{
                list->next = (StudentList*) malloc(sizeof(StudentList));
                list = list->next;
                list->sem = current->sem;
                strcpy(list->dept , current->dept);
                strcpy(list->name , current->name);
                strcpy(list->rno , current->rno);
                list->next=NULL;
            }
            // printf("%d  %f........\n" ,current->this_attendance->numOfClasses , current->this_attendance->percentageAttendance );
        }
        current=current->next;
    }while(current!=last->next);
    return list1;
}
typedef struct mcc
{
    char dept[2];
    int sem;
    char course_name[50];
    int max_classes_conducted;
    int highest_classes_attended;
    int pos;
    struct mcc* next;
} Mcc;


Course* insert_course(Mcc **mcc_head, Course *head, char course_name[], int sem, char dept[], int num_classes) 
{
    Course* new_course=(Course*) malloc(sizeof(Course));
    strcpy(new_course->course_name, course_name);
    new_course->sem=sem;
    strcpy(new_course->dept, dept);
    new_course->num_classes=num_classes;
    new_course->next=NULL;
    if(head==NULL) 
    {
        head=new_course;
    }
    else
    {
        Course *current=head;
        Course *previous=NULL;
        // traversing until the dept and the sem becomes same.
        while(current!=NULL && strcmp(current->dept, new_course->dept) < 0)
        {
            previous=current;
            current=current->next;
        }
        while(current!=NULL && strcmp(current->dept, new_course->dept)==0 && current->sem < new_course->sem) 
        {
            previous=current;
            current=current->next;
        }
        if(previous==NULL)
        {
            // inserting at start
            new_course->next=head;
            head=new_course;
        }   
        else 
        {
            //inserting after prev
            previous->next=new_course;
            new_course->next=current;
        }
    }
    Mcc *mcc_node=*mcc_head;
    int status=0;
    while(mcc_node!=NULL && status==0) 
    {
        if(mcc_node->sem==sem && strcmp(mcc_node->dept, dept)==0) 
        {
            if(num_classes > mcc_node->max_classes_conducted)  // changing couurse name and the mum of classes
            {
                mcc_node->max_classes_conducted=num_classes;
                strcpy(mcc_node->course_name , course_name);
            }
            status=1;
        }
        if(status != 1)
		{
            mcc_node = mcc_node->next;
        }
    }
    if(mcc_node==NULL ) 
    {
        Mcc* new_mcc=(Mcc*) malloc(sizeof(Mcc));
        strcpy(new_mcc->dept, dept);
        strcpy(new_mcc->course_name, new_course->course_name);
        new_mcc->sem=sem;
        new_mcc->max_classes_conducted=num_classes;
        new_mcc->highest_classes_attended=0;
        new_mcc->next=*mcc_head;
        *mcc_head=new_mcc;
        
    }
    return head;
}


void add_student(Mcc **mcc_head, Course* head, Student** last, char name[], char dept[], int sem, char rno[], int classes_attended[]) 
{
    Student* new_student=(Student*)malloc(sizeof(Student));
    strcpy(new_student->name, name);
    strcpy(new_student->dept, dept);
    new_student->sem=sem;
    strcpy(new_student->rno,rno);
    new_student->this_attendance = (Attendance*)malloc(sizeof(Attendance));
    
    strcpy(new_student->this_attendance->dept,dept);
    strcpy(new_student->this_attendance->rno,rno);
    int i;
    Course *current=head;
    Mcc *mcc_current=*mcc_head;
    while(strcmp(current->dept, dept) < 0)
    {
        current=current->next;
    }
    while(strcmp(current->dept, new_student->dept)==0 && current->sem < sem) 
    {
        current=current->next;
    }
    while(strcmp(mcc_current->dept, new_student->dept)!=0 || mcc_current->sem!=sem)
    {
        mcc_current=mcc_current->next;
    }
    for(i=0;i<5;i++)
    {
        new_student->classes_conducted[i]=current->num_classes;
        // printf("%s\n" , current->course_name);
        if(strcmp(current->course_name,mcc_current->course_name)==0)
        {
            mcc_current->pos=i;
            (new_student->this_attendance)->numOfClasses = classes_attended[i];
            // new_student->this_attendance->percentageAttendance = classes_attended[i] * (1.0/mcc_current->highest_classes_attended)*100;

        }
        current=current->next;
    }
    for(i=0;i<5;i++)
    {
        new_student->classes_attended[i]=classes_attended[i];
        if(i==mcc_current->pos)
        {
            if(new_student->classes_attended[i]>mcc_current->highest_classes_attended)
            {
                mcc_current->highest_classes_attended=new_student->classes_attended[i];
                // printf("%s %d %d     /////" , mcc_current->dept , mcc_current->sem , mcc_current->highest_classes_attended);
            }
        }
    }
    new_student->next=NULL;
    new_student->this_attendance->next=NULL;
    if(*last==NULL) 
    {
        *last=new_student;
        (*last)->next=*last;
    }
    else
    {
        new_student->next=(*last)->next;
        (*last)->next=new_student;
        *last=new_student;  
    }
    Student* temp = (*last)->next;
}

Attendance** create_attend_list(Mcc *mcc_head, Student* last)
{
	Attendance **arr=(Attendance**) malloc(4*sizeof(Attendance*));
    int i;
    for (i=0;i<4;i++) 
    {
    	arr[i]=NULL;
    }
    if(last==NULL)
    {
        printf("Student record list is empty");
    }
    else
    {
        Student* current=last->next;
        do
        {
            Mcc* temp = mcc_head;
            while(temp!=NULL && (strcmp(temp->dept,current->dept)!=0 || current->sem!=temp->sem))
            {
                temp=temp->next;
            }
            current->this_attendance->percentageAttendance=current->classes_attended[temp->pos]*(1.0/temp->highest_classes_attended)*100;
            current->this_attendance->next=arr[(current->sem)/2-1];
            arr[(current->sem)/2-1]=current->this_attendance;
            current=current->next;
        } while (current!=last->next);
    }
    return arr;
}

void updatePercentageAttendance(Student* last  , Mcc *mcc_head){
    Student* current = last->next;
    while(current!=last){
        Mcc* temp = mcc_head;
        // while(temp!=NULL && strcmp(temp->dept,current->dept)!=0 && current->sem!=temp->sem){
        //     temp=temp->next;
        // }
        int status = 0 ;
        while(temp!=NULL && status==0){
            if(strcmp(temp->dept,current->dept)==0 && current->sem==temp->sem){
                status = 1;
            }
            if(status==0)
                temp = temp->next;
        }
        // while(temp!=NULL && current->sem!=temp->sem){
        //     temp=temp->next;
        // }
        // while(temp!=NULL && strcmp(temp->dept,current->dept)!=0){
        //     temp=temp->next;
        // }
        // printf("%d %d %d %s %d" ,temp->pos , current->classes_attended[temp->pos] ,  temp->highest_classes_attended , temp->dept , temp->sem);
        current->this_attendance->percentageAttendance=current->classes_attended[temp->pos]*(1.0/temp->highest_classes_attended)*100;
        current=current->next;
    }
        Mcc* temp = mcc_head;
        while(temp!=NULL && strcmp(temp->dept,current->dept)!=0 && current->sem!=temp->sem){
            temp=temp->next;
        }
        current->this_attendance->percentageAttendance=current->classes_attended[temp->pos]*(1.0/temp->highest_classes_attended)*100;
        current=current->next;
}
void add_fee(Fee** last, char rno[], char status[]) 
{
    Fee* new_fee=(Fee*)malloc(sizeof(Fee));
    strcpy(new_fee->rno,rno);
    strcpy(new_fee->status,status);
    new_fee->next=NULL;
    if(*last==NULL) 
    {
        *last=new_fee;
        (*last)->next=*last;
    }
    else
    {
        new_fee->next=(*last)->next;
        (*last)->next=new_fee;
        *last=new_fee;  
    }
}


Fee* create_fee_list(Student* last)
{
    Fee* fee_last=NULL;
    if(last==NULL)
    {
        printf("Student record list is empty");
    }
    else
    {
        Student* current=last->next;
        char fee_status[7];
        FILE* fptr = fopen("feestatus.txt" , "r");
        while(current!=last)
        {
            printf("Enter the fee status of %s : " , current->rno);
            fscanf( fptr, "%s", fee_status);
            printf(" %s\n" , fee_status);
            add_fee(&fee_last,current->rno,fee_status);
            current->this_fee = fee_last;
            current=current->next;
        }
        printf("Enter the fee status of %s\n" , current->rno);
        fscanf( fptr, "%s", fee_status);
        printf(" %s\n" , fee_status);
        add_fee(&fee_last,current->rno,fee_status);
        current->this_fee = fee_last;
        fclose(fptr) ;
    }
    return fee_last;
}


void add_application(Applicant** last, char name[], char rno[], char status[]) 
{
    Applicant* new_applicant=(Applicant*)malloc(sizeof(Applicant));
    strcpy(new_applicant->name,name);
    strcpy(new_applicant->rno,rno);
    strcpy(new_applicant->status,status);
    new_applicant->next=NULL;
    if(*last==NULL) 
    {
        *last=new_applicant;
        (*last)->next=*last;
    }
    else
    {
        new_applicant->next=(*last)->next;
        (*last)->next=new_applicant;
        *last=new_applicant;  
    }
}


Applicant* create_applicant_list(Student* last)
{
    Applicant* applicant_last=NULL;
    if(last==NULL)
    {
        printf("Student record list is empty");
    }
    else
    {
        FILE *fptr = fopen("appstatus.txt" , "r");
        Student* current=last->next;
        char application_status[2];
        while(current!=last)
        {
            printf("Enter the application status of %s : " , current->rno);
            fscanf( fptr, "%s", application_status);
            printf("%s\n" , application_status);
            add_application(&applicant_last,current->name,current->rno,application_status);
            current->this_application = applicant_last;
            current=current->next;
        }
        printf("Enter the application status of %s : " , current->rno);
        fscanf( fptr, "%s", application_status);
        printf("%s\n" , application_status);
        add_application(&applicant_last,current->name,current->rno,application_status);
        current->this_application = applicant_last;
    }
    return applicant_last;
}


//attendence list - array of linked list


Student* divide(Student* lptr)
{

	Student *nptr,*fast,*slow;
	slow=lptr;
	fast=lptr->next->next;
	while(fast!=NULL)
	{
		fast=fast->next;
		slow=slow->next;
		if(fast!=NULL)
		{
			fast=fast->next;
		}
	}
	nptr=slow->next;
	slow->next=NULL;
	return nptr;
}

Student* merge(Student* lptr, Student* nptr)
{
	Student *result, *ptr1, *ptr2, *tail;
	ptr1=lptr;
	ptr2=nptr;
	if(ptr1->sem < ptr2->sem)
	{
		result=ptr1;
		ptr1=ptr1->next;
	}
	else if(ptr1->sem > ptr2->sem)
	{
		result=ptr2;
		ptr2=ptr2->next;
	}
    else
    {
        if(strcmp(ptr1->dept,ptr2->dept)<0)
        {
            result=ptr1;
            ptr1=ptr1->next;
        }
        else if(strcmp(ptr1->dept,ptr2->dept)>0)
        {
            result=ptr2;
            ptr2=ptr2->next;
        }
        else
        {
            if(strcmp(ptr1->rno,ptr2->rno)<0)
            {
                result=ptr1;
                ptr1=ptr1->next;
            }
            else
            {
                result=ptr2;
                ptr2=ptr2->next;
            }
        }    
    }
	tail=result;
	while((ptr1!=NULL) && (ptr2!=NULL))
	{
        if(ptr1->sem < ptr2->sem)
        {
            tail=tail->next=ptr1;
			ptr1=ptr1->next;
        }
        else if(ptr1->sem > ptr2->sem)
        {
            tail=tail->next=ptr2;
			ptr2=ptr2->next;
        }
        else
        {
            if(strcmp(ptr1->dept,ptr2->dept)<0)
            {
                tail=tail->next=ptr1;  
			    ptr1=ptr1->next;
            }
            else if(strcmp(ptr1->dept,ptr2->dept)>0)
            {
                tail=tail->next=ptr2;
			    ptr2=ptr2->next;
            }
            else
            {
                if(strcmp(ptr1->rno,ptr2->rno)<0)
                {
                    tail=tail->next=ptr1;
			        ptr1=ptr1->next;
                }
                else
                {
                    tail=tail->next=ptr2;
			        ptr2=ptr2->next;
                }
            }
	    }
    }
	if(ptr1!=NULL)
		tail->next=ptr1;
	else
		tail->next=ptr2;
	return result;
}

Student* mergesort(Student* list_ptr)
{
	Student *nptr,*lptr;
	lptr=list_ptr;
	if((lptr!=NULL) && (lptr->next!=NULL))
	{
		nptr=divide(lptr);
		lptr=mergesort(lptr);
		nptr=mergesort(nptr);
		lptr=merge(lptr,nptr);
	}
	return lptr;
} 

Student* student_sort(Student* last)
{
    Student* head=last->next;
    last->next=NULL;
    head=mergesort(head);
    Student* current=head;
    while(current->next!=NULL)
    {
        current=current->next;
    }
    last=current;
    last->next=head;
    return last;
}


void print_apply_na(Student* last)
{
    if(last==NULL)
	{
		printf("Student record list is empty");
	}
	else
	{
		Student *current=last->next;
        int status=0;
        printf("      Name                                       Sem   Dept\n");
		while(current!=last)
		{
            if(strcmp(current->this_application->status,"NA")==0)
            {
                printf("%-50s %-5d %-5s\n",current->name,current->sem,current->dept);
            }
			current=current->next;
		}
        if(strcmp(current->this_application->status,"NA")==0)
        {
            printf("%-50s %-5d %-5s\n",current->name,current->sem,current->dept);
        }
	}
}


void print(Student *last)
{
	if(last==NULL)
	{
		printf("Student record list is empty");
	}
	else
	{
		Student *current=last->next;
		while(current!=last)
		{
			printf("%s\n",current->name); //need to add other details
			current=current->next;
		}
        printf("%s\n",current->name);
	}
}

void print_MoreAttendencePendingStatus(Student* last){
    Student* current = last->next;
    do{
        if(current->this_attendance->percentageAttendance>75 && strcmp(current->this_fee->status,"pending")==0){
            printf("%s\n" , current->name);
        }
        current=current->next;
    }while(current!=last->next);
}

StudentList*  CreatelistOfDefaulters(Student* last){
    Student* current = last->next;
    StudentList* list = NULL;
    StudentList* list1 = list;
    int NumOfStudents[5] = {0,0,0,0,0};
    
    do{
        if(current->this_attendance->percentageAttendance<=75 || strcmp(current->this_fee->status,"pending")==0){
            if(list==NULL){
                list = (StudentList*) malloc(sizeof(StudentList));
                list1=list;
            }else{
                list->next = (StudentList*) malloc(sizeof(StudentList));
                list = list->next;
            }
            list->sem = current->sem;
            strcpy(list->dept , current->dept);
            strcpy(list->name , current->name);
            strcpy(list->rno , current->rno);
            list->next=NULL;
            if(strcmp(current->dept , "CS")==0){
                NumOfStudents[0]++;
            }else if(strcmp(current->dept , "EE")==0){
                NumOfStudents[1]++;
            }else if(strcmp(current->dept , "EC")==0){
                NumOfStudents[2]++;
            }else if(strcmp(current->dept , "ME")==0){
                NumOfStudents[3]++;
            }
            else if(strcmp(current->dept , "CV")==0){
                NumOfStudents[4]++;
            }
        }
        current=current->next;
    }while(current!=last->next);
    int ind = 0 ;
    for(int i = 0 ; i<5 ; i++){
        if(NumOfStudents[i]>NumOfStudents[ind]){
            ind = i;
        }
    }
    switch (ind)
    {
    case 0:
        printf("Max is in CS dept!\n");
        break;
    case 1:
        printf("Max is in EE dept!\n");
        break;
    case 2:
        printf("Max is in EC dept!\n");
        break;
    case 3:
        printf("Max is in ME dept!\n");
        break;
    default:
        printf("Max is in CV dept!\n");
        break;
    }
    return list1;
}
int main()
{
    Course* course= NULL;
    Student* s = NULL;
    Mcc* mcc = NULL;
    FILE *filePointer ;
     
 
    filePointer = fopen("courses.txt", "r") ;
    if(filePointer==NULL){
        printf("Doesn't Exist!!\n");
    }
    char course_name[7];
    int sem;
    int classesConducted;
    char dept[3];
    while(fscanf(filePointer , "%s %d %s %d", course_name , &sem , dept , &classesConducted)!=EOF){
        course=insert_course(&mcc , course , course_name , sem ,dept , classesConducted);
    }
    fclose(filePointer) ;
    
    filePointer = fopen("students.txt", "r") ;
    if(filePointer==NULL){
        printf("Doesn't Exist!!\n");
    }
    char name[50];
    char rno[11];
    int attendanceOfStud[5];
    char temp[50];
  
    while(fscanf(filePointer , "%s" , temp)!=EOF){
        strcpy(name , "");
        int flag = 0;
        while(flag==0){
            strcat(name , temp);
            fscanf(filePointer , "%s" , temp);
            if(temp[0]=='B' && temp[1]=='T'){
                flag=1;
            }else{
                strcat(name , " ");
            }
        }
        strcpy(rno , temp);
        fscanf(filePointer , "%s %d %d %d %d %d %d", dept , &sem, &attendanceOfStud[4] , &attendanceOfStud[3] , &attendanceOfStud[2] , &attendanceOfStud[1] , &attendanceOfStud[0] );
        add_student(&mcc,course , &s , name , dept , sem , rno ,attendanceOfStud);
    }
    fclose(filePointer) ;


    updatePercentageAttendance(s , mcc);
   
    
    // while(mcc!=NULL){
    //     printf("%s %d %d %d mmmmmmmmmmm\n" , mcc->dept , mcc->sem , mcc->pos , mcc->highest_classes_attended);
    //     mcc=mcc->next;
    // }
    Fee* fee_list =  create_fee_list(s);
    Applicant* appllicant_ist = create_applicant_list(s);
// Q1
    s = student_sort(s);
//Q2  Display year wise and then the department-wise names of the students who did not apply for the examination
    printf("\nyear wise and then the department-wise names of the students who did not apply for the examination:\n ");
    print_apply_na(s);
// Q3
    printf("\nCreated a list of all eligible students for the semester examination.\n");
    StudentList* listOfEligibleStudents =  CreatListOfEligibleStudents(s);
    while(listOfEligibleStudents!=NULL){
        printf("%s\n" , listOfEligibleStudents->name );
        listOfEligibleStudents=listOfEligibleStudents->next;
    }


//Q4
    printf("\nCreated a list of students having attendance <=75%% .\n");
    StudentList* listOfStudentsHavingLessThanOrEqTo75 =  lessThanOrEqTo75(s);
    while(listOfStudentsHavingLessThanOrEqTo75!=NULL){
        printf("%s\n" , listOfStudentsHavingLessThanOrEqTo75->name);
        listOfStudentsHavingLessThanOrEqTo75=listOfStudentsHavingLessThanOrEqTo75->next;
    }

//Q5
    printf("\nCreated a list of students having attendance >75%% but feee status pending  .\n");
    print_MoreAttendencePendingStatus(s);

//Q6
    printf("\nList of Defaulters : \n");
    StudentList* listOfDefaulters = CreatelistOfDefaulters(s);
     while(listOfDefaulters!=NULL){
        printf("%-30s %s\n" , listOfDefaulters->name , listOfDefaulters->rno);
        listOfDefaulters=listOfDefaulters->next;
    }
	printf("\n");
    // print(s);
    // printf("\n");
    

    Attendance** arrayy;
    arrayy = create_attend_list(mcc , s);
    printf("YO\n");
    while(arrayy[0]!=NULL){
        printf("%s %f\n" , arrayy[0]->rno , arrayy[0]->percentageAttendance);
        arrayy[0]=arrayy[0]->next;
    }
    return 0;
}
