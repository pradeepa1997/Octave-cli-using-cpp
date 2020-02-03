#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;


struct Matrix{
    int col; //number of col in matrix
    int row; //number of rows in matrix
    int ** data;
    // pointer to pointer store matrix data in 2d array size wil assign dinamacaly when the code run
};

struct dataStructure{
    string vname; //variable name
    Matrix *dataptr;
    dataStructure *next;
    dataStructure * pre;
    // void pointer for store relevent data it can be scalars , vectors matrix
};
dataStructure * head;
dataStructure * tail;

void removeSpace(string &input){
    int len =input.length();
    for( int i = 0; i < len; i++)
    {
        if (input[i] == ' ')
        {
            for (int j = i; j < len; j++)
                input[j] = input[j+1];
            input.erase(len-1,1);
            len--;
            
        }

    }
    input.erase(len);
    
}
dataStructure * findVariable(string vname){
    if(head==NULL)return NULL;
    dataStructure * temp=head;
    while(temp){
        if(temp->vname==vname){
            return temp;
        }
        temp=temp->next;
    }
    return NULL;

}
void printOutput(dataStructure * data){
    if(!data)return;
    cout<<data->vname<<":\n\t";
    for(int i=0;i<data->dataptr->row;i++){
        for(int j=0;j<data->dataptr->col;j++){
            cout<<data->dataptr->data[i][j]<<"\t";
        }
        cout<<endl<<"\t";
    }
    cout<<endl;
}
void printOutput(string input){
    dataStructure * temp;
    if(head==NULL){
        cout<<"error: "<<input<<" undefined "<<endl<<endl;
        return;
    }
    temp=findVariable(input);
    if(temp){
        printOutput(temp);
    }else{
        cout<<"error: "<<input<<" undefined "<<endl<<endl;
    }
    
    
    
}
void deleteVariable(dataStructure * data){
    if(head==tail){
        tail=head=NULL;
        return;
    }
    if(data==head){
        head=head->next;
        head->pre=NULL;
        return;
    }
    if(data==tail){
        tail=tail->pre;
        tail->next=NULL;
        return;
    }

    data->pre->next=data->next;
    data->next->pre=data->pre;
    free(data);

}
dataStructure* insertToMemory(Matrix * newmatrix,string vname){

    dataStructure *newslot;
    if(newslot=findVariable(vname)){
        deleteVariable(newslot);
    }
    newslot=new dataStructure();
    newslot->dataptr=newmatrix;
    newslot->next=newslot->pre=NULL;
    newslot->vname=vname;
    if(head==NULL){
        head=newslot;
        tail=newslot;
        
    }else{
        tail->next=newslot;
        newslot->pre=tail;
        tail=newslot;
    }
    return newslot;
}

dataStructure * createVariable(string vname,dataStructure    * data){
    if(!data)return NULL;
    return insertToMemory(data->dataptr,vname);
    
}

int cast(string data){
    char arr[data.length()+1];
    strcpy(arr,data.c_str());
    return atoi(arr);
}


Matrix * createMatrix(string input){
    // cout<<input<<endl;
    if(input[input.length()-2]==','){
       input[input.length()-2]=';'; 
    }else if(input[input.length()-2]==';'){
    }else{
        if(!(input.find(',')==-1 && input.find(';')==-1)){
            input[input.length()-1]=';';
            input.append("]");
        }else{
            input.append(";");
        }
        
    }
    
    
    int element=0,row=1,col=0;
    for(int i=0;i<input.length();i++){
       if(input[i]==';'){
           col++;
           break;
        }
       if(input[i]==',')col++;
       
    }
    
    for(int i=0;i<input.length()-2;i++){
        
       if(input[i]==';'){
           row++;
           element++;
        }
       if(input[i]==',')element++;
       
    }
    
    element++;
    if((input[0]!='[' || input[input.length()-1]!=']') && element>1){
        cout<<"Syntax error1"<<endl;
        return NULL;
    }
    if(input.find('[')!=-1){
        // cout<<input<<endl;
        input.erase(0,1);
        input.erase(input.length()-1,1);
        
    }
    // cout<<input<<endl;
    // cout<<"r:"<<row<<"c:"<<col<<"e:"<<element<<endl;
    
    if(element/row!=col){
        cout<<"Syantax error"<<endl<<endl;
        return NULL;
    }
    Matrix *newVar=new Matrix();
    newVar->col=col;
    newVar->row=row;
    newVar->data=new int*[row];
    for(int i=0;i<row;i++){
        newVar->data[i]=new int[col];
    }
    int i=0;
    for(int ir=0;ir<newVar->row;ir++){
        int findex=input.find(';');
        string rowstr;
        if(findex!=input.length()-1){
            rowstr=input.substr(0,findex);
            input=input.substr(findex+1,input.length()-findex-1);
        }else{
            rowstr=input.substr(0,input.length()-1);
        }
        rowstr.append(",");
        for(int ic=0;ic<newVar->col;ic++){
            findex=rowstr.find(",");
            string val=rowstr.substr(0,findex);
            rowstr=rowstr.substr(findex+1,rowstr.length()-findex-1);
            newVar->data[ir][ic]=cast(val);

        }
    }
    return newVar;
}

dataStructure * createVariable(string input){
    // extract variable name
    
    int equ=input.find("=");
    
    string variblename=input.substr(0,equ);
    // sepearating only values of the data
    input=input.substr(equ+1,input.length()-equ-1);
    
    Matrix * newMat=createMatrix(input);
    
    if(newMat){
        
        return (insertToMemory(newMat,variblename));
    }else{
        return NULL;
    }
    
    
    
}
dataStructure* substract(string input){
    int findIndex=input.find('-');
    string m1=input.substr(0,findIndex);
    string m2=input.substr(findIndex+1,input.length()-findIndex-1);
    // cout<<m1<<endl;
    // cout<<m2<<endl;
    dataStructure *M1,*M2;
    if(m1.find(',')==-1 && m1.find(';')==-1){
        M1=findVariable(m1);
        if(!M1)return NULL;
    }else{
        M1=new dataStructure();
        M1->dataptr=createMatrix(m1);
        
    }
    if(m2.find(',')==-1 && m2.find(';')==-1){
        M2=findVariable(m2);
        if(!M2)return NULL;
    }else{
        M2=new dataStructure();
        M2->dataptr=createMatrix(m2);

    }
    
    if(M1->dataptr->col!=M2->dataptr->col||M1->dataptr->row!=M2->dataptr->row){
        cout<<"Don't match coloum or row sizes"<<endl<<endl;
        return NULL;
    }
    Matrix * result=new Matrix();
    result->col=M1->dataptr->col;
    result->row=M1->dataptr->row;
    result->data=new int*[result->row];
    for(int i=0;i<M1->dataptr->row;i++){
        result->data[i]=new int[result->col];
        for(int j=0;j<M1->dataptr->col;j++){
            result->data[i][j]=M1->dataptr->data[i][j] - M2->dataptr->data[i][j];
        }
    }
    dataStructure * ans=new dataStructure();
    ans->vname="";
    ans->dataptr=result;
    return ans;
}
dataStructure* add(string input){
    int findIndex=input.find('+');
    string m1=input.substr(0,findIndex);
    string m2=input.substr(findIndex+1,input.length()-findIndex-1);
    // cout<<m1<<endl;
    // cout<<m2<<endl;
    dataStructure *M1,*M2;
    if(m1.find(',')==-1 && m1.find(';')==-1){
        M1=findVariable(m1);
        if(!M1)return NULL;
    }else{
        M1=new dataStructure();
        M1->dataptr=createMatrix(m1);
        
    }
    if(m2.find(',')==-1 && m2.find(';')==-1){
        M2=findVariable(m2);
        if(!M2)return NULL;
    }else{
        M2=new dataStructure();
        M2->dataptr=createMatrix(m2);

    }
    
    if(M1->dataptr->col!=M2->dataptr->col||M1->dataptr->row!=M2->dataptr->row){
        cout<<"Don't match coloum or row sizes"<<endl<<endl;
        return NULL;
    }
    Matrix * result=new Matrix();
    result->col=M1->dataptr->col;
    result->row=M1->dataptr->row;
    result->data=new int*[result->row];
    for(int i=0;i<M1->dataptr->row;i++){
        result->data[i]=new int[result->col];
        for(int j=0;j<M1->dataptr->col;j++){
            result->data[i][j]=M1->dataptr->data[i][j] + M2->dataptr->data[i][j];
        }
    }
    dataStructure * ans=new dataStructure();
    ans->vname="";
    ans->dataptr=result;
    return ans;
}
dataStructure* elementViseMultiplication(string input){
    int findIndex=input.find('*');
    string m1=input.substr(0,findIndex-1);
    string m2=input.substr(findIndex+1,input.length()-findIndex-1);
    // cout<<m1<<endl;
    // cout<<m2<<endl;
    dataStructure *M1,*M2;
    if(m1.find(',')==-1 && m1.find(';')==-1){
        M1=findVariable(m1);
        if(!M1){
            char arr[m1.length()+1];
            strcpy(arr,m1.c_str());
            int val=atoi(arr);
            if(!val) return NULL;
            M1=new dataStructure();
            M1->dataptr=createMatrix(m1);

        }
    }else{
        M1=new dataStructure();
        M1->dataptr=createMatrix(m1);
        
    }
    if(m2.find(',')==-1 && m2.find(';')==-1){
        if(m1.find('['))
        M2=findVariable(m2);
        if(!M2){
            char arr[m2.length()+1];
            strcpy(arr,m2.c_str());
            int val=atoi(arr);
            if(!val) return NULL;
            M2=new dataStructure();
            M2->dataptr=createMatrix(m2);

        }
    }else{
        M2=new dataStructure();
        M2->dataptr=createMatrix(m2);

    }
    
    if(M1->dataptr->col!=M2->dataptr->col||M1->dataptr->row!=M2->dataptr->row){
        
        if(M2->dataptr->col==1 && M2->dataptr->row==1){
            Matrix * result=new Matrix();
            result->col=M1->dataptr->col;
            result->row=M1->dataptr->row;
            result->data=new int*[result->row];
            for(int i=0;i<M1->dataptr->row;i++){
                result->data[i]=new int[result->col];
                for(int j=0;j<M1->dataptr->col;j++){
                    result->data[i][j]=M1->dataptr->data[i][j] * M2->dataptr->data[0][0];
                }
            }
            dataStructure * ans=new dataStructure();
            ans->vname="";
            ans->dataptr=result;
            return ans; 

        }else if(M1->dataptr->col==1 && M1->dataptr->row==1){
            Matrix * result=new Matrix();
            result->col=M1->dataptr->col;
            result->row=M1->dataptr->row;
            result->data=new int*[result->row];
            for(int i=0;i<M1->dataptr->row;i++){
                result->data[i]=new int[result->col];
                for(int j=0;j<M1->dataptr->col;j++){
                    result->data[i][j]=M1->dataptr->data[i][j] * M2->dataptr->data[0][0];
                }
            }
            dataStructure * ans=new dataStructure();
            ans->vname="";
            ans->dataptr=result;
            return ans; 
        }else if(M1->dataptr->col==M2->dataptr->col){
            if(M2->dataptr->row==1){
                Matrix * result=new Matrix();
                result->col=M1->dataptr->col;
                result->row=M1->dataptr->row;
                result->data=new int*[result->row];
                for(int i=0;i<M1->dataptr->row;i++){
                    result->data[i]=new int[result->col];
                    for(int j=0;j<M1->dataptr->col;j++){
                        result->data[i][j]=M1->dataptr->data[i][j] * M2->dataptr->data[0][j];
                    }
                }
                dataStructure * ans=new dataStructure();
                ans->vname="";
                ans->dataptr=result;
                return ans;
            }else if(M1->dataptr->row==1){

                Matrix * result=new Matrix();
                result->col=M1->dataptr->col;
                result->row=M1->dataptr->row;
                result->data=new int*[result->row];
                for(int i=0;i<M1->dataptr->row;i++){
                    result->data[i]=new int[result->col];
                    for(int j=0;j<M1->dataptr->col;j++){
                        result->data[i][j]=M1->dataptr->data[0][j] * M2->dataptr->data[i][j];
                    }
                }
                dataStructure * ans=new dataStructure();
                ans->vname="";
                ans->dataptr=result;
                return ans;

            }

        }else if(M1->dataptr->row==M2->dataptr->row){
            if(M1->dataptr->col==1){
                Matrix * result=new Matrix();
                result->col=M2->dataptr->col;
                result->row=M2->dataptr->row;
                result->data=new int*[result->row];
                for(int i=0;i<M2->dataptr->row;i++){
                    result->data[i]=new int[result->col];
                    for(int j=0;j<M2->dataptr->col;j++){
                        result->data[i][j]=M1->dataptr->data[i][0] * M2->dataptr->data[i][j];
                    }
                }
                dataStructure * ans=new dataStructure();
                ans->vname="";
                ans->dataptr=result;
                return ans;
            }else if(M2->dataptr->col=1){
                Matrix * result=new Matrix();
                result->col=M1->dataptr->col;
                result->row=M1->dataptr->row;
                result->data=new int*[result->row];
                for(int i=0;i<M1->dataptr->row;i++){
                    result->data[i]=new int[result->col];
                    for(int j=0;j<M1->dataptr->col;j++){
                        result->data[i][j]=M1->dataptr->data[i][j] * M2->dataptr->data[i][0];
                    }
                }
                dataStructure * ans=new dataStructure();
                ans->vname="";
                ans->dataptr=result;
                return ans;
            }
        }
    }else{
      
        Matrix * result=new Matrix();
        result->col=M1->dataptr->col;
        result->row=M1->dataptr->row;
        result->data=new int*[result->row];
        for(int i=0;i<M1->dataptr->row;i++){
            result->data[i]=new int[result->col];
            for(int j=0;j<M1->dataptr->col;j++){
                result->data[i][j]=M1->dataptr->data[i][j] * M2->dataptr->data[i][j];
            }
        }
        dataStructure * ans=new dataStructure();
        ans->vname="";
        ans->dataptr=result;
        return ans;
    }
    cout<<"Colmn or rows not match"<<endl<<endl;
    return NULL;
    
}
dataStructure* multiplication(string input){
    int findIndex=input.find('*');
    string m1=input.substr(0,findIndex);
    string m2=input.substr(findIndex+1,input.length()-findIndex-1);
    
    dataStructure *M1,*M2;
    if(m1.find(',')==-1 && m1.find(';')==-1){
        M1=findVariable(m1);
        if(!M1)return NULL;
    }else{
        M1=new dataStructure();
        M1->dataptr=createMatrix(m1);
        
    }
    if(m2.find(',')==-1 && m2.find(';')==-1){
        M2=findVariable(m2);
        if(!M2)return NULL;
    }else{
        M2=new dataStructure();
        M2->dataptr=createMatrix(m2);

    }
    
    if(M1->dataptr->col!=M2->dataptr->row){
        cout<<"Don't match coloum or row sizes"<<endl<<endl;
    }
    Matrix * result=new Matrix();
    result->col=M2->dataptr->col;
    result->row=M1->dataptr->row;
    result->data=new int*[result->row];
    for(int i=0;i<result->row;i++){
        result->data[i]=new int[result->col];
        for(int j=0;j<result->col;j++){
            int sum=0;
            for(int  k=0;k<M1->dataptr->col;k++){
                sum=sum+(M1->dataptr->data[i][k]*M2->dataptr->data[k][j]);

            }
            result->data[i][j]=sum;
        }
    }
    dataStructure * ans=new dataStructure();
    ans->vname="";
    ans->dataptr=result;
    return ans;
    
}
void oparate(string input){
    int equal=input.find('=');
    dataStructure * temp;
    if(equal==-1){
        
        if(input.find('-')!=-1){
            temp=substract(input);
        }else if(input.find('+')!=-1){
           temp=add(input); 
        }else if((input.find('.')!=-1)&&(input.find('*')!=-1)){
            temp=elementViseMultiplication(input);

        }else if(input.find('*')!=-1){
            temp=multiplication(input);
        }else{
            printOutput(input);
            temp=NULL;
        }

    }else{
         
        
        if(input.find('-')!=-1){
            // passing assign variable name and equation separatly
            temp=createVariable(input.substr(0,equal),substract(input.substr(equal+1,input.length()-1-equal)));

        }else if(input.find('+')!=-1){
             
            temp=createVariable(input.substr(0,equal),add(input.substr(equal+1,input.length()-1-equal)));

        }else if((input.find('.')!=-1)&&(input.find('*')!=-1)){

            temp=createVariable(input.substr(0,equal),elementViseMultiplication(input.substr(equal+1,input.length()-1-equal)));
        }else if(input.find('*')!=-1){
            
            temp=createVariable(input.substr(0,equal),multiplication(input.substr(equal+1,input.length()-1-equal)));
        }else{
            temp=createVariable(input);
        }
        
    }
    printOutput(temp);
    
}


int main(){
    head=NULL;
    cout<<"Welcome to Universal Complex Scripting Calculator by UCSC, Sri Lanka."<<endl<<"Version: 1.0"<<endl;
    while (1){
        cout<<">>";
        string input;
        getline(cin,input);
        removeSpace(input);
        if(input=="exit")exit(1);
        oparate(input);

        
        
    }
    
   
}