#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define NUM_OF_COURSES 100
#define NUM_OF_DEPT 5
#define NUM_OF_SEM 4
// Structure for general student record
typedef struct  
{
    char courseName[10];
    int semester;
    char dept[3];
    int numOfClassesConducted;

} Course;

typedef struct 
{
    Course* MccCourse;
    int highestClassesAttended;
    int index;
} MCC;

// Structure for attendance record
typedef struct {
    char rollNumber[11];
    char department[3];
    int classesAttended;
    float attendancePercentage;
} AttendanceRecord;

// Structure for fee status record
typedef struct {
    char rollNumber[11];
    char feeStatus[8];
} FeeRecord;

// Structure for applicant record
typedef struct {
    char name[50];
    char rollNumber[11];
    char applicantStatus[3];
} ApplicantRecord;

typedef struct {
    char name[50];
    char department[3];
    int year;
    int semester;
    char rollNumber[11];
    int numClassesConducted[5];
    int numClassesAttended[5];
    AttendanceRecord* thisAttendance;
    FeeRecord* thisFee;
    ApplicantRecord* thisApp;
} StudentRecord;


//AVL Nodes for diff data
typedef struct Node1
{
    StudentRecord stuRec;
    struct Node1* left;
    struct Node1* right;

}AVLStudentRecordNode;

typedef struct Node2
{
    AttendanceRecord* stuAtt;
    struct Node2* left;
    struct Node2* right;

}AVLAttendanceRecordNode;

typedef struct Node3
{
    FeeRecord* stuFee;
    struct Node3* left;
    struct Node3* right;

}AVLFeeRecordNode;

typedef struct Node4
{
    ApplicantRecord* stuApp;
    struct Node4* left;
    struct Node4* right;

}AVLAppRecordNode;
void updateCourseArray(Course courseArray[] ,FILE* fptr ){
    char courseName[10];
    int semester;
    char dept[3];
    int numOfClassesConducted;
    for(int  i = 0 ; i<NUM_OF_COURSES ; i++){
        fscanf(fptr , "%s %d %s %d", courseName , &semester , dept , &numOfClassesConducted);
        Course temp ;
        strcpy(temp.courseName , courseName);
        strcpy(temp.dept , dept);
        temp.semester = semester;
        temp.numOfClassesConducted = numOfClassesConducted;
        courseArray[i] = temp;
    }
}
void updateMccArray(MCC MccArray[] , Course courseArray[] ){
    for(int i = 0 ; i<NUM_OF_COURSES ; i+=5){
        for(int j = 0 ; j<5 ; j++){
            if(j==0){
                MccArray[i/5].MccCourse = &courseArray[i+j];
                MccArray[i/5].index = j;
                MccArray[i/5].highestClassesAttended = 0;
            }else if(courseArray[i+j].numOfClassesConducted > MccArray[i/5].MccCourse->numOfClassesConducted){
                MccArray[i/5].MccCourse = &courseArray[i+j];
                MccArray[i/5].index = j;
                MccArray[i/5].highestClassesAttended = 0;
            }
        }
    }
}


int compareStudentRecord(StudentRecord key1 , StudentRecord key2){
    if(key1.semester<key2.semester  ||  (key1.semester==key2.semester && strcmp(key1.department , key2.department)<0) || (key1.semester==key2.semester && strcmp(key1.department , key2.department)==0 && strcmp(key1.rollNumber , key2.rollNumber)<0)){
        return -1;
    }else if(   key1.semester>key2.semester || (key1.semester==key2.semester && strcmp(key1.department , key2.department)>0)  ||                                        (key1.semester==key2.semester && strcmp(key1.department , key2.department)==0 && strcmp(key1.rollNumber , key2.rollNumber)>0)){
        return 1;
    }

    return 0;
}
AVLStudentRecordNode* rightRotate(AVLStudentRecordNode* y)
{
    AVLStudentRecordNode* x = y->left;
    AVLStudentRecordNode* T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    return x;
}
AVLStudentRecordNode* leftRotate(AVLStudentRecordNode* x)
{
    AVLStudentRecordNode* y = x->right;
    AVLStudentRecordNode* T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    return y;
}
int max(int x, int y){
    if(x > y){
        return x;
    }else{
        return y;
    }
}
int height(AVLStudentRecordNode* N){
    if(N==NULL)return 0 ;

    return 1  + max(height(N->left) , height(N->right));
}
int getBalance(AVLStudentRecordNode* N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
AVLStudentRecordNode* insertStudentRecordInAVL(AVLStudentRecordNode* node ,StudentRecord key ){
    if (node == NULL){
        node =  (AVLStudentRecordNode*)malloc(sizeof(AVLStudentRecordNode)) ;
        node->stuRec = key;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
 
        // printf("%s\n" , key.name);
    if (compareStudentRecord(key , node->stuRec)<0)
        node->left  = insertStudentRecordInAVL(node->left, key);
    else if (compareStudentRecord(key , node->stuRec)>0)
        node->right = insertStudentRecordInAVL(node->right, key);
    else
        return node;
 
    
    int balance = getBalance(node);
 
    // Left Left Case
    if (balance > 1 && compareStudentRecord(key , node->left->stuRec)<0)
        return rightRotate(node);
 
    // Right Right Case
    if (balance < -1 && compareStudentRecord(key , node->right->stuRec)>0)
        return leftRotate(node);
 
    // Left Right Case
    if (balance > 1 && compareStudentRecord(key , node->left->stuRec)>0)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
 
    // Right Left Case
    if (balance < -1 && compareStudentRecord(key ,node->right->stuRec)<0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
 
    return node;
}
AVLStudentRecordNode* updateStudentRecord(AVLStudentRecordNode* studentRoot ,FILE* fptr , Course courseArray[] ,  MCC MccArray[]){
    char tempString[50];
    while(fscanf(fptr , "%s" , tempString)!=EOF){
        StudentRecord temp;
        strcpy(temp.name , "");
        int flag = 0;
        while(flag==0){
            strcat(temp.name , tempString);
            fscanf(fptr , "%s" , tempString);
            if(tempString[0]=='B' && tempString[1]=='T'){
                flag=1;
            }else{
                strcat(temp.name , " ");
            }
        }
        strcpy(temp.rollNumber , tempString);
        fscanf(fptr , "%s %d %d %d %d %d %d", temp.department , &temp.semester, &temp.numClassesAttended[0] , &temp.numClassesAttended[1] , &temp.numClassesAttended[2] , &temp.numClassesAttended[3] , &temp.numClassesAttended[4] );

        int ind = 0;
        while(courseArray[ind].semester!=temp.semester || strcmp(courseArray[ind].dept , temp.department)!=0){
            ind++;
        }
        for(int i = ind ; i<ind+5 ; i++){
            temp.numClassesConducted[i-ind] = courseArray[i].numOfClassesConducted;
        }
        AttendanceRecord* tempAttend = (AttendanceRecord*)malloc(sizeof(AttendanceRecord));
        strcpy(tempAttend->department , temp.department);
        strcpy(tempAttend->rollNumber , temp.rollNumber);
        tempAttend->classesAttended = temp.numClassesAttended[MccArray[ind/5].index];
        
        tempAttend->attendancePercentage = 0.0f;

        //updating the max classes attended by any student for mcc
        if(tempAttend->classesAttended > MccArray[ind/5].highestClassesAttended){
            MccArray[ind/5].highestClassesAttended = tempAttend->classesAttended;
        }
        temp.thisAttendance = tempAttend;
        studentRoot = insertStudentRecordInAVL(studentRoot , temp);
    }
    return studentRoot;
}
void inOrderStudentRecord(AVLStudentRecordNode* root){
    if(root!=NULL){
        inOrderStudentRecord(root->left);
        printf("%-30s %-15s %d %d %d %d %d %.2f\n" , root->stuRec.name , root->stuRec.rollNumber , root->stuRec.numClassesAttended[0] , root->stuRec.numClassesAttended[1] , root->stuRec.numClassesAttended[2] , root->stuRec.numClassesAttended[3] , root->stuRec.numClassesAttended[4] , root->stuRec.thisAttendance->attendancePercentage);
        inOrderStudentRecord(root->right);
    }
}


int height2(AVLAttendanceRecordNode* N){
    if(N==NULL)return 0 ;

    return 1  + max(height2(N->left) , height2(N->right));
}
int getBalance2(AVLAttendanceRecordNode* N)
{
    if (N == NULL)
        return 0;
    return height2(N->left) - height2(N->right);
}
AVLAttendanceRecordNode* rightRotate2(AVLAttendanceRecordNode* y)
{
    AVLAttendanceRecordNode* x = y->left;
    AVLAttendanceRecordNode* T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    return x;
}
AVLAttendanceRecordNode* leftRotate2(AVLAttendanceRecordNode* x)
{
    AVLAttendanceRecordNode* y = x->right;
    AVLAttendanceRecordNode* T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    return y;
}
AVLAttendanceRecordNode* insertAttendanceRecord(AVLAttendanceRecordNode* node , AttendanceRecord* key){
    if (node == NULL){
        node =  (AVLAttendanceRecordNode*)malloc(sizeof(AVLAttendanceRecordNode)) ;
        node->stuAtt = key;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
 
        // printf("%s\n" , key.name);
    if (strcmp(key->rollNumber , node->stuAtt->rollNumber)<0)
        node->left  = insertAttendanceRecord(node->left, key);
    else if (strcmp(key->rollNumber , node->stuAtt->rollNumber)>0)
        node->right = insertAttendanceRecord(node->right, key);
    else
        return node;
 
    
    int balance = getBalance2(node);
 
    // Left Left Case
    if (balance > 1 && strcmp(key->rollNumber , node->left->stuAtt->rollNumber)<0)
        return rightRotate2(node);
 
    // Right Right Case
    if (balance < -1 && strcmp(key->rollNumber , node->right->stuAtt->rollNumber)>0)
        return leftRotate2(node);
 
    // Left Right Case
    if (balance > 1 && strcmp(key->rollNumber , node->left->stuAtt->rollNumber)>0)
    {
        node->left =  leftRotate2(node->left);
        return rightRotate2(node);
    }
 
    // Right Left Case
    if (balance < -1 && strcmp(key->rollNumber ,node->right->stuAtt->rollNumber)<0)
    {
        node->right = rightRotate2(node->right);
        return leftRotate2(node);
    }
 
    return node;
}
AVLAttendanceRecordNode* updateAttendanceRecord(AVLAttendanceRecordNode* currSemAtten ,AVLStudentRecordNode* studentRoot , int semester , MCC MccArray[]){
    if(studentRoot!=NULL){
        AVLStudentRecordNode* curr = studentRoot;
        if(curr->stuRec.semester > semester){
            currSemAtten = updateAttendanceRecord(currSemAtten , studentRoot->left , semester , MccArray);
        }else if(curr->stuRec.semester < semester){
            currSemAtten = updateAttendanceRecord(currSemAtten , studentRoot->right , semester , MccArray);
        }else{

            int ind = 0 ;
            //search for mccSubject for the current student
            while (ind<NUM_OF_DEPT*NUM_OF_SEM &&( MccArray[ind].MccCourse->semester!=studentRoot->stuRec.semester || strcmp(MccArray[ind].MccCourse->dept , studentRoot->stuRec.department)!=0))
            {
                ind++;
            }
            AttendanceRecord* tempAttend = studentRoot->stuRec.thisAttendance;
            float percentage =(float) ((tempAttend->classesAttended)*100.0)/MccArray[ind].highestClassesAttended;
         
            tempAttend->attendancePercentage = percentage;
            
            currSemAtten = insertAttendanceRecord(currSemAtten , studentRoot->stuRec.thisAttendance);
            currSemAtten = updateAttendanceRecord(currSemAtten , studentRoot->left , semester , MccArray);
            currSemAtten = updateAttendanceRecord(currSemAtten , studentRoot->right , semester , MccArray);
           
        }
    }
    return currSemAtten;
}
void inOrderAttendance(AVLAttendanceRecordNode* root){
    if(root!=NULL){
        inOrderAttendance(root->left);
        printf("%-15s %.2f\n"  , root->stuAtt->rollNumber, root->stuAtt->attendancePercentage );
        inOrderAttendance(root->right);
    }
}

AVLFeeRecordNode* rightRotate3(AVLFeeRecordNode* y)
{
    AVLFeeRecordNode* x = y->left;
    AVLFeeRecordNode* T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    return x;
}
AVLFeeRecordNode* leftRotate3(AVLFeeRecordNode* x)
{
    AVLFeeRecordNode* y = x->right;
    AVLFeeRecordNode* T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    return y;
}
int height3(AVLFeeRecordNode* N){
    if(N==NULL)return 0 ;

    return 1  + max(height3(N->left) , height3(N->right));
}
int getBalance3(AVLFeeRecordNode* N)
{
    if (N == NULL)
        return 0;
    return height3(N->left) - height3(N->right);
}
AVLFeeRecordNode* insertFeeRecord(AVLFeeRecordNode* node ,FeeRecord* key ){
    if (node == NULL){
        node =  (AVLFeeRecordNode*)malloc(sizeof(AVLFeeRecordNode)) ;
        node->stuFee = key;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
 
    if (strcmp(key->rollNumber , node->stuFee->rollNumber)<0)
        node->left  = insertFeeRecord(node->left, key);
    else if (strcmp(key->rollNumber , node->stuFee->rollNumber)>0)
        node->right = insertFeeRecord(node->right, key);
    else
        return node;
 
    
    int balance = getBalance3(node);
 
    // Left Left Case
    if (balance > 1 && strcmp(key->rollNumber , node->left->stuFee->rollNumber)<0)
        return rightRotate3(node);
 
    // Right Right Case
    if (balance < -1 && strcmp(key->rollNumber , node->right->stuFee->rollNumber)>0)
        return leftRotate3(node);
 
    // Left Right Case
    if (balance > 1 && strcmp(key->rollNumber , node->left->stuFee->rollNumber)>0)
    {
        node->left =  leftRotate3(node->left);
        return rightRotate3(node);
    }
 
    // Right Left Case
    if (balance < -1 && strcmp(key->rollNumber ,node->right->stuFee->rollNumber)<0)
    {
        node->right = rightRotate3(node->right);
        return leftRotate3(node);
    }
 
    return node;
}
AVLFeeRecordNode* readFeeStatus(AVLFeeRecordNode* FeeRec ,AVLStudentRecordNode* studentRoot ,FILE* fptr){
    if(studentRoot!=NULL){
        FeeRec = readFeeStatus(FeeRec , studentRoot->left ,fptr);
        AVLStudentRecordNode* current = studentRoot;
       
        char feeStatus[8];
        
        printf("Enter the fee status of %s : " , current->stuRec.rollNumber);
        fscanf( fptr, "%s", feeStatus);
        printf(" %s\n" , feeStatus);
        FeeRecord* currFee = NULL;
        currFee = (FeeRecord*) malloc(sizeof(FeeRecord));
        strcpy(currFee->rollNumber , current->stuRec.rollNumber);
        strcpy(currFee->feeStatus , feeStatus);
        current->stuRec.thisFee = currFee;
        FeeRec = insertFeeRecord(FeeRec , currFee);
        FeeRec = readFeeStatus(FeeRec , studentRoot->right ,fptr);

    }
    return FeeRec;
}
void inOrderFeeRecord(AVLFeeRecordNode* root){
    if(root!=NULL){
        inOrderFeeRecord(root->left);
        printf("%-15s %-15s\n" ,root->stuFee->rollNumber , root->stuFee->feeStatus );
        inOrderFeeRecord(root->right);
    }
}

AVLAppRecordNode* rightRotate4(AVLAppRecordNode* y)
{
    AVLAppRecordNode* x = y->left;
    AVLAppRecordNode* T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    return x;
}
AVLAppRecordNode* leftRotate4(AVLAppRecordNode* x)
{
    AVLAppRecordNode* y = x->right;
    AVLAppRecordNode* T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    return y;
}
int height4(AVLAppRecordNode* N){
    if(N==NULL)return 0 ;

    return 1  + max(height4(N->left) , height4(N->right));
}
int getBalance4(AVLAppRecordNode* N)
{
    if (N == NULL)
        return 0;
    return height4(N->left) - height4(N->right);
}
AVLAppRecordNode* insertAppRecord(AVLAppRecordNode* node ,ApplicantRecord* key ){
    if (node == NULL){
        node =  (AVLAppRecordNode*)malloc(sizeof(AVLAppRecordNode)) ;
        node->stuApp = key;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
 
        // printf("%s\n" , key.name);
    if (strcmp(key->rollNumber , node->stuApp->rollNumber)<0)
        node->left  = insertAppRecord(node->left, key);
    else if (strcmp(key->rollNumber , node->stuApp->rollNumber)>0)
        node->right = insertAppRecord(node->right, key);
    else
        return node;
 
    
    int balance = getBalance4(node);
 
    // Left Left Case
    if (balance > 1 && strcmp(key->rollNumber , node->left->stuApp->rollNumber)<0)
        return rightRotate4(node);
 
    // Right Right Case
    if (balance < -1 && strcmp(key->rollNumber , node->right->stuApp->rollNumber)>0)
        return leftRotate4(node);
 
    // Left Right Case
    if (balance > 1 && strcmp(key->rollNumber , node->left->stuApp->rollNumber)>0)
    {
        node->left =  leftRotate4(node->left);
        return rightRotate4(node);
    }
 
    // Right Left Case
    if (balance < -1 && strcmp(key->rollNumber ,node->right->stuApp->rollNumber)<0)
    {
        node->right = rightRotate4(node->right);
        return leftRotate4(node);
    }
 
    return node;
}
AVLAppRecordNode* readAppStatus(AVLAppRecordNode* AppRec ,AVLStudentRecordNode* studentRoot ,FILE* fptr){
    if(studentRoot!=NULL){
        AppRec = readAppStatus(AppRec , studentRoot->left ,fptr);
        AVLStudentRecordNode* current = studentRoot;
       
        char appStatus[3];
        
        printf("Enter the application status of %s : " , current->stuRec.rollNumber);
        fscanf( fptr, "%s", appStatus);
        printf(" %s\n" , appStatus);
        ApplicantRecord* currApp = NULL;
        currApp = (ApplicantRecord*) malloc(sizeof(ApplicantRecord));
        strcpy(currApp->rollNumber , current->stuRec.rollNumber);
        strcpy(currApp->applicantStatus , appStatus);
        current->stuRec.thisApp = currApp;
        AppRec = insertAppRecord(AppRec , currApp);
        AppRec = readAppStatus(AppRec , studentRoot->right ,fptr);

    }
    return AppRec;
}
void inOrderAppRecord(AVLAppRecordNode* root){
    if(root!=NULL){
        inOrderAppRecord(root->left);
        printf("%-15s %-15s\n" ,root->stuApp->rollNumber , root->stuApp->applicantStatus );
        inOrderAppRecord(root->right);
    }
}


void displayNamesWhoDidNotApplyForExam(AVLStudentRecordNode* studentRoot){
    if(studentRoot!=NULL){
        displayNamesWhoDidNotApplyForExam(studentRoot->left);
        if(strcmp(studentRoot->stuRec.thisApp->applicantStatus , "NA")==0){
            printf("%-20s %-2d %-5s\n" , studentRoot->stuRec.name , studentRoot->stuRec.semester ,studentRoot->stuRec.department );
        }
        displayNamesWhoDidNotApplyForExam(studentRoot->right);
    }
}
int countNumOfEligibleStudent(AVLStudentRecordNode* studentRoot){
    int ans = 0 ;   
    if(studentRoot!=NULL){
        ans += countNumOfEligibleStudent(studentRoot->left);
        ans += countNumOfEligibleStudent(studentRoot->right);
        if(studentRoot->stuRec.thisAttendance->attendancePercentage>75.0f && strcmp(studentRoot->stuRec.thisApp->applicantStatus,"A")==0 && strcmp(studentRoot->stuRec.thisFee->feeStatus , "clear")==0){
            ans++;
        }
    }
    return ans;
}
void updateEligibileList(char** eligibleList  , int* ind ,AVLStudentRecordNode* studentRoot ){
    if(studentRoot!=NULL){
        updateEligibileList(eligibleList, ind , studentRoot->left);
        if(studentRoot->stuRec.thisAttendance->attendancePercentage>75.0f && strcmp(studentRoot->stuRec.thisApp->applicantStatus,"A")==0 && strcmp(studentRoot->stuRec.thisFee->feeStatus , "clear")==0){
            strcpy(eligibleList[*ind] , studentRoot->stuRec.name);
            (*ind)++;
        }
        updateEligibileList(eligibleList  , ind ,studentRoot->right); 
    }
}

int countNumOfStudentHavingLessThanEqualTo75(AVLStudentRecordNode* studentRoot){
    int ans = 0 ;   
    if(studentRoot!=NULL){
        ans += countNumOfStudentHavingLessThanEqualTo75(studentRoot->left);
        ans += countNumOfStudentHavingLessThanEqualTo75(studentRoot->right);
        if(studentRoot->stuRec.thisAttendance->attendancePercentage<=75.0f){
            ans++;
        }
    }
    return ans;
}
void updatelessThanEqualTo75List(char** lessThanEqualTo75List  , int* ind ,AVLStudentRecordNode* studentRoot ){
    if(studentRoot!=NULL){
        updatelessThanEqualTo75List(lessThanEqualTo75List, ind , studentRoot->left);
        if(studentRoot->stuRec.thisAttendance->attendancePercentage<=75.0f){
            
            strcpy(lessThanEqualTo75List[*ind] , studentRoot->stuRec.name);
            (*ind)++;
        }
        updatelessThanEqualTo75List(lessThanEqualTo75List  , ind ,studentRoot->right); 
    }
}
void printNameOfStudentsHavingAttendanceMoreThan75ButFeePending(AVLStudentRecordNode* studentRoot ){
    if(studentRoot!=NULL){
        printNameOfStudentsHavingAttendanceMoreThan75ButFeePending(studentRoot->left);
        if(studentRoot->stuRec.thisAttendance->attendancePercentage>75.0f && strcmp(studentRoot->stuRec.thisFee->feeStatus , "pending")==0){
            printf("%-20s\n" , studentRoot->stuRec.name);
        }
        printNameOfStudentsHavingAttendanceMoreThan75ButFeePending(studentRoot->right); 
    }
}
AVLStudentRecordNode* IOS(AVLStudentRecordNode* studentRoot ){
    while(studentRoot->left!=NULL){
        studentRoot = studentRoot->left;
    }
    return studentRoot;
}
AVLStudentRecordNode* deleteStudentRecordFromAVL(AVLStudentRecordNode* studentRoot , char rollNum[]  , int* sem){
    if(studentRoot!=NULL){
        if(strcmp(studentRoot->stuRec.rollNumber , rollNum )==0){
            *sem = studentRoot->stuRec.semester;
            // free(studentRoot->stuRec.thisApp);
            // free(studentRoot->stuRec.thisAttendance);
            // free(studentRoot->stuRec.thisFee);
            // studentRoot->stuRec.thisApp = NULL;
            // studentRoot->stuRec.thisAttendance = NULL;
            // studentRoot->stuRec.thisFee = NULL;

            if(studentRoot->left==NULL){
                AVLStudentRecordNode* retVal = studentRoot->right;
                free(studentRoot);
                return retVal;
            }else if(studentRoot->right==NULL){
                AVLStudentRecordNode* retVal = studentRoot->left;
                free(studentRoot);
                return retVal;
            }else{
                AVLStudentRecordNode* nextNode = IOS(studentRoot->right);
                studentRoot->stuRec = nextNode->stuRec;
                int IOSSem = 0;
                studentRoot->right = deleteStudentRecordFromAVL(studentRoot->right ,nextNode->stuRec.rollNumber ,  &IOSSem  );
                return studentRoot;
            }
        }else{
            studentRoot->left = deleteStudentRecordFromAVL(studentRoot->left , rollNum , sem);
            if(*sem==0){
                studentRoot->right = deleteStudentRecordFromAVL(studentRoot->right , rollNum , sem);
            }
        }
        int balance = getBalance(studentRoot);
 
        // Left Left Case
        if (balance > 1 && getBalance(studentRoot->left) >= 0)
            return rightRotate(studentRoot);
 
        // Left Right Case
        if (balance > 1 && getBalance(studentRoot->left) < 0)
        {
            studentRoot->left = leftRotate(studentRoot->left);
            return rightRotate(studentRoot);
        }
 
        // Right Right Case
        if (balance < -1 && getBalance(studentRoot->right) <= 0)
            return leftRotate(studentRoot);
 
        // Right Left Case
        if (balance < -1 && getBalance(studentRoot->right) > 0)
        {
            studentRoot->right = rightRotate(studentRoot->right);
            return leftRotate(studentRoot);
        }
 
        return studentRoot;
    }
    return NULL;
}


AVLAppRecordNode* IOS4(AVLAppRecordNode* AppRec ){
    while(AppRec->left!=NULL){
        AppRec = AppRec->left;
    }
    return AppRec;
}
AVLAppRecordNode* deleteAppRecordFromAVL( AVLAppRecordNode* AppRec , char rollNum[] , int *done){
    if(AppRec!=NULL){
        if(strcmp(AppRec->stuApp->rollNumber , rollNum )==0){
            *done=1;
            if(AppRec->left==NULL){
                AVLAppRecordNode* retVal = AppRec->right;
                free(AppRec);
                return retVal;
            }else if(AppRec->right==NULL){
                AVLAppRecordNode* retVal = AppRec->left;
                free(AppRec);
                return retVal;
            }else{
                AVLAppRecordNode* nextNode = IOS4(AppRec->right);
                AppRec->stuApp = nextNode->stuApp;
                int IOSSem = 0;
                AppRec->right = deleteAppRecordFromAVL(AppRec->right ,nextNode->stuApp->rollNumber, &IOSSem );
                return AppRec;
            }
        }else{
            AppRec->left = deleteAppRecordFromAVL(AppRec->left , rollNum,done );
            if(*done==0){
                AppRec->right = deleteAppRecordFromAVL(AppRec->right , rollNum , done);
            }
        }
        int balance = getBalance4(AppRec);
 
        // Left Left Case
        if (balance > 1 && getBalance4(AppRec->left) >= 0)
            return rightRotate4(AppRec);
 
        // Left Right Case
        if (balance > 1 && getBalance4(AppRec->left) < 0)
        {
            AppRec->left = leftRotate4(AppRec->left);
            return rightRotate4(AppRec);
        }
 
        // Right Right Case
        if (balance < -1 && getBalance4(AppRec->right) <= 0)
            return leftRotate4(AppRec);
 
        // Right Left Case
        if (balance < -1 && getBalance4(AppRec->right) > 0)
        {
            AppRec->right = rightRotate4(AppRec->right);
            return leftRotate4(AppRec);
        }
 
        return AppRec;
    }
    return NULL;
}

AVLFeeRecordNode* IOS3(AVLFeeRecordNode* FeeRec ){
    while(FeeRec->left!=NULL){
        FeeRec = FeeRec->left;
    }
    return FeeRec;
}
AVLFeeRecordNode* deleteFeeRecordFromAVL( AVLFeeRecordNode* FeeRec , char rollNum[] , int *done){
    if(FeeRec!=NULL){
        if(strcmp(FeeRec->stuFee->rollNumber , rollNum )==0){
            *done=1;
            if(FeeRec->left==NULL){
                AVLFeeRecordNode* retVal = FeeRec->right;
                free(FeeRec);
                return retVal;
            }else if(FeeRec->right==NULL){
                AVLFeeRecordNode* retVal = FeeRec->left;
                free(FeeRec);
                return retVal;
            }else{
                AVLFeeRecordNode* nextNode = IOS3(FeeRec->right);
                FeeRec->stuFee = nextNode->stuFee;
                int done1 = 0;
                FeeRec->right = deleteFeeRecordFromAVL(FeeRec->right ,nextNode->stuFee->rollNumber, &done1 );
                return FeeRec;
            }
        }else{
            FeeRec->left = deleteFeeRecordFromAVL(FeeRec->left , rollNum,done );
            if(*done==0){
                FeeRec->right = deleteFeeRecordFromAVL(FeeRec->right , rollNum , done);
            }
        }
        int balance = getBalance3(FeeRec);
 
        // Left Left Case
        if (balance > 1 && getBalance3(FeeRec->left) >= 0)
            return rightRotate3(FeeRec);
 
        // Left Right Case
        if (balance > 1 && getBalance3(FeeRec->left) < 0)
        {
            FeeRec->left = leftRotate3(FeeRec->left);
            return rightRotate3(FeeRec);
        }
 
        // Right Right Case
        if (balance < -1 && getBalance3(FeeRec->right) <= 0)
            return leftRotate3(FeeRec);
 
        // Right Left Case
        if (balance < -1 && getBalance3(FeeRec->right) > 0)
        {
            FeeRec->right = rightRotate3(FeeRec->right);
            return leftRotate3(FeeRec);
        }
 
        return FeeRec;
    }
    return NULL;
}


AVLAttendanceRecordNode* IOS2(AVLAttendanceRecordNode* FeeRec ){
    while(FeeRec->left!=NULL){
        FeeRec = FeeRec->left;
    }
    return FeeRec;
}
AVLAttendanceRecordNode* deleteAttendanceRecordFromAVL( AVLAttendanceRecordNode* attendRec , char rollNum[] , int *done){
    if(attendRec!=NULL){
        if(strcmp(attendRec->stuAtt->rollNumber , rollNum )==0){
            *done=1;
            if(attendRec->left==NULL){
                AVLAttendanceRecordNode* retVal = attendRec->right;
                free(attendRec);
                return retVal;
            }else if(attendRec->right==NULL){
                AVLAttendanceRecordNode* retVal = attendRec->left;
                free(attendRec);
                return retVal;
            }else{
                AVLAttendanceRecordNode* nextNode = IOS2(attendRec->right);
                attendRec->stuAtt = nextNode->stuAtt;
                int done1 = 0;
                attendRec->right = deleteAttendanceRecordFromAVL(attendRec->right ,nextNode->stuAtt->rollNumber, &done1 );
                return attendRec;
            }
        }else{
            attendRec->left = deleteAttendanceRecordFromAVL(attendRec->left , rollNum,done );
            if(*done==0){
                attendRec->right = deleteAttendanceRecordFromAVL(attendRec->right , rollNum , done);
            }
        }
        int balance = getBalance2(attendRec);
 
        // Left Left Case
        if (balance > 1 && getBalance2(attendRec->left) >= 0)
            return rightRotate2(attendRec);
 
        // Left Right Case
        if (balance > 1 && getBalance2(attendRec->left) < 0)
        {
            attendRec->left = leftRotate2(attendRec->left);
            return rightRotate2(attendRec);
        }
 
        // Right Right Case
        if (balance < -1 && getBalance2(attendRec->right) <= 0)
            return leftRotate2(attendRec);
 
        // Right Left Case
        if (balance < -1 && getBalance2(attendRec->right) > 0)
        {
            attendRec->right = rightRotate2(attendRec->right);
            return leftRotate2(attendRec);
        }
 
        return attendRec;
    }
    return NULL;
}


void deleteRecord(char rollNum[] , AVLStudentRecordNode** studentRoot , AVLAppRecordNode** AppRec , AVLFeeRecordNode** FeeRec , AVLAttendanceRecordNode** attendance2ndSem , AVLAttendanceRecordNode** attendance4thdSem , AVLAttendanceRecordNode** attendance6thSem  , AVLAttendanceRecordNode** attendance8thSem){

    int sem = 0;
    *studentRoot = deleteStudentRecordFromAVL(*studentRoot , rollNum  , &sem);
    if(sem==0){
        printf("RollNum Not Found!\n");
        return;
    }
    int done = 0;
    *AppRec = deleteAppRecordFromAVL(*AppRec , rollNum , &done);
    done = 0;
    *FeeRec = deleteFeeRecordFromAVL(*FeeRec , rollNum ,&done);
    done = 0;
    if(sem == 2){
        *attendance2ndSem = deleteAttendanceRecordFromAVL(*attendance2ndSem, rollNum , &done);
    }else if(sem == 4){
        *attendance4thdSem = deleteAttendanceRecordFromAVL(*attendance4thdSem, rollNum , &done);
    }else if(sem == 6){
        *attendance6thSem = deleteAttendanceRecordFromAVL(*attendance6thSem , rollNum , &done);
    }else{
        *attendance8thSem = deleteAttendanceRecordFromAVL(*attendance8thSem , rollNum , &done);
    }
    printf("Record Deleted\n");
}
int main(){
    Course courseArray[NUM_OF_COURSES];

    FILE* fptr = fopen("courses.txt", "r");
    updateCourseArray(courseArray , fptr);
    fclose(fptr) ;
    
    printf("------------------Courses--------------------\n");
    for(int i = 0 ; i<NUM_OF_COURSES ; i++)
        printf("%-10s %-2d\n" , courseArray[i].courseName , courseArray[i].semester);


    MCC MccArray[NUM_OF_DEPT*NUM_OF_SEM];
    updateMccArray(MccArray , courseArray);
    printf("----------------MCC Subjects------------------\n");
    for(int i = 0 ; i<NUM_OF_DEPT*NUM_OF_SEM ; i++)
        printf("%-10s %-2d %-2d\n" , MccArray[i].MccCourse->courseName , MccArray[i].MccCourse->semester , MccArray[i].highestClassesAttended);

    
    AVLStudentRecordNode* studentRoot = NULL;
    fptr = fopen("students.txt", "r") ;
    studentRoot = updateStudentRecord(studentRoot , fptr ,courseArray ,  MccArray);
    

    AVLAttendanceRecordNode* attendance2ndSem = NULL;
    attendance2ndSem = updateAttendanceRecord(attendance2ndSem , studentRoot , 2 , MccArray);
    inOrderAttendance(attendance2ndSem);
    AVLAttendanceRecordNode* attendance4thdSem = NULL;
    attendance4thdSem = updateAttendanceRecord(attendance4thdSem , studentRoot , 4 , MccArray);
    inOrderAttendance(attendance4thdSem);
    AVLAttendanceRecordNode* attendance6thSem = NULL;
    attendance6thSem = updateAttendanceRecord(attendance6thSem , studentRoot , 6 , MccArray);
    inOrderAttendance(attendance6thSem);
    AVLAttendanceRecordNode* attendance8thSem = NULL;
    attendance8thSem = updateAttendanceRecord(attendance8thSem , studentRoot , 8 , MccArray);
    inOrderAttendance(attendance8thSem);


    printf("------------------Students--------------------\n");
    inOrderStudentRecord(studentRoot);
    AVLFeeRecordNode* FeeRec = NULL;
    fptr = fopen("feestatus.txt", "r") ;
    FeeRec = readFeeStatus(FeeRec , studentRoot , fptr);
    fclose(fptr) ;
    // inOrderFeeRecord(FeeRec);


    AVLAppRecordNode* AppRec = NULL;
    fptr = fopen("appstatus.txt", "r") ;
    AppRec = readAppStatus(AppRec , studentRoot , fptr);
    fclose(fptr) ;
    // inOrderAppRecord(AppRec);

    //Q2
    printf("names of the students who did not apply for the examination : \n");
    printf("name               sem dept\n");
    displayNamesWhoDidNotApplyForExam(studentRoot);

    printf("-----------------------------------------------------------------------------------------------------------\n");

    //Q3
    int numOfEligibleStudent = countNumOfEligibleStudent(studentRoot);
    char** eligibleList = (char**)malloc(numOfEligibleStudent * sizeof(char*)) ;
    for(int i = 0 ; i<numOfEligibleStudent ; i++){
        eligibleList[i] = (char*)malloc(sizeof(char)*50);
    }
    int index = 0;
    updateEligibileList(eligibleList , &index , studentRoot);
    printf("%d students are Eligible for Exam  :\n" , numOfEligibleStudent);
    for(int i = 0 ; i<numOfEligibleStudent ; i++){
        printf("%-20s\n" , eligibleList[i]);
    }

    printf("-----------------------------------------------------------------------------------------------------------\n");
    //Q4
    int numOfStudentHavingLessThanEqualTo75 = countNumOfStudentHavingLessThanEqualTo75(studentRoot);
    char** lessThanEqualTo75List = (char**)malloc(numOfStudentHavingLessThanEqualTo75 * sizeof(char*)) ;
    for(int i = 0 ; i<numOfStudentHavingLessThanEqualTo75 ; i++){
        lessThanEqualTo75List[i] = (char*)malloc(sizeof(char)*50);
    }
     index = 0;
    updatelessThanEqualTo75List(lessThanEqualTo75List , &index , studentRoot);
    printf("%d students have <=75%% attendance  :\n" , numOfStudentHavingLessThanEqualTo75);
    for(int i = 0 ; i<numOfStudentHavingLessThanEqualTo75 ; i++){
        printf("%-20s\n" , lessThanEqualTo75List[i]);
    }


    printf("-----------------------------------------------------------------------------------------------------------\n");
    //Q5 Delete a record
    char rollNum[11];
    printf("Enter the rollNumber of the student whose data needs to be deleted :");
    scanf("%s" , rollNum);
    deleteRecord(rollNum , &studentRoot , &AppRec , &FeeRec , &attendance2ndSem , &attendance4thdSem , &attendance6thSem  , &attendance8thSem);
    // inOrderStudentRecord(studentRoot);
    // inOrderAttendance(attendance6thSem);

    printf("-----------------------------------------------------------------------------------------------------------\n");
    //Q6
    printf("Students having attendance more than 75 but pending fee stattus : \n");
    printNameOfStudentsHavingAttendanceMoreThan75ButFeePending(studentRoot);


}

