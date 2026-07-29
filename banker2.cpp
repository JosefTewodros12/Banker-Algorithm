#include <iostream>
using namespace std;
//structure of the resource
struct resource {
    int rid;
    int amount;
    resource *next=NULL;
};
resource *rlist=NULL, *freelist=NULL, *tempresourcelist=NULL, *totallist=NULL;

struct process {
    int pid;
    resource *maxlist=NULL;
    resource *allocatedlist=NULL;
    resource *needlist=NULL;
    process *next=NULL;
};

process *plist=NULL;

// function Prototypes
void initialize();
void safechecker();
void calculateneed();
void calculatefreeresources();
int check_safe_state();

void addlasttoresourcelist(resource *t) {   
    resource *temp, *temp1;
    temp = tempresourcelist;
    if(temp == NULL)
        tempresourcelist = t;
    else {
        temp1 = tempresourcelist;
        while(temp1->next != NULL)
            temp1 = temp1->next;
        temp1->next = t; //cout<<endl<<t->amount<<endl;
    }
}


void addlastprocess(process *temp) {
    process *temp1;
    temp1 = plist;
    if(plist == NULL)
        plist = temp;
    else {
        while(temp1->next != NULL)
            temp1 = temp1->next;
        temp1->next = temp;   
    }
}
void calculatefreeresources() {
    
    resource *total_ptr = totallist;
    int r_id = 1;//is becuase linkedlist is calculated by the using of node number 1,2,3
    
    while(total_ptr != NULL) {
        int allocated_sum = 0;//accumulator for calculating of allocated sum
        process *p_ptr = plist;
        
        // traverse all processes to sum up what is currently allocated for this resource type
        while(p_ptr != NULL) {
            resource *alloc_ptr = p_ptr->allocatedlist;
            int current_rid = 1;//traverse allocating lst
            while(alloc_ptr != NULL && current_rid < r_id) {
                alloc_ptr = alloc_ptr->next;
                current_rid++;
            }
            if(alloc_ptr != NULL) {
                allocated_sum += alloc_ptr->amount;
            }
            p_ptr = p_ptr->next;
        }
        
        // Free = Total - Allocated
        resource *free_node = new resource;
        free_node->rid = r_id;//this node represent the R1++
        //this one calculate the free instance or resource
        free_node->amount = total_ptr->amount - allocated_sum;
        free_node->next = NULL;
        
        //  add a new node to the end of freelist
        if(freelist == NULL) {
            freelist = free_node;
        } else {
            resource *temp = freelist;
            while(temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = free_node;
        }
        total_ptr = total_ptr->next;
        r_id++;
    }
}
int check_safe_state() {
    
    int num_processes = 0;
    process *p_temp = plist;
    while(p_temp != NULL) {
        num_processes++;
        p_temp = p_temp->next;
    }
    // 0 = process not finsihed, 1 process is finished
    int *finish = new int[num_processes];
    for(int i = 0; i < num_processes; i++) {
        finish[i] = 0; 
    }
    // coping of free list to work list
    resource *work = NULL;
    resource *f_temp = freelist;
    while(f_temp != NULL) {
        resource *w_node = new resource;
        w_node->rid = f_temp->rid;
        w_node->amount = f_temp->amount;
        w_node->next = NULL;
        // add copied to the final node
        if(work == NULL) work = w_node;
        else {
            resource *w_temp = work;
            while(w_temp->next != NULL) w_temp = w_temp->next;
            w_temp->next = w_node;
        }
        f_temp = f_temp->next;
    }

    bool found_process;
    do {
        found_process = false;
        p_temp = plist;
        int p_index = 0;
        
        while(p_temp != NULL) {//Check only unfinished processes
            if(finish[p_index] == 0) {
                
                // check if Need <= Work
                bool can_finish = true;
                resource *need_ptr = p_temp->needlist;
                resource *work_ptr = work;
                
                while(need_ptr != NULL && work_ptr != NULL) {
                    if(need_ptr->amount > work_ptr->amount) {
                        can_finish = false; // cannot finish, needs too much
                        break;
                    }
                    need_ptr = need_ptr->next;
                    work_ptr = work_ptr->next;
                }

                // Work = Work + Allocated
                if(can_finish) {
                    resource *w_ptr = work;
                    resource *a_ptr = p_temp->allocatedlist;
                    while(w_ptr != NULL && a_ptr != NULL) {
                        w_ptr->amount += a_ptr->amount;
                        w_ptr = w_ptr->next;
                        a_ptr = a_ptr->next;
                    }
                    finish[p_index] = 1; 
                    found_process = true;
                }
            }
            p_temp = p_temp->next;
            p_index++;
        }
    } while(found_process);//Outer loop continues until no process can finish in a full scan

    // the system is in an UNSAFE state (Return 0). Otherwise, it is SAFE (Return 1).
    int isSafe = 1; 
    for(int i = 0; i < num_processes; i++) {
        if(finish[i] == 0) {
            isSafe = 0; 
            break;
        }
    }
    // clean up dynamic memory to prevent leaks
    delete[] finish;
    while(work != NULL) {
        resource *temp_w = work;
        work = work->next;
        delete temp_w;
    }

    return isSafe;
}
// safe checker
void safechecker() {
    int state = check_safe_state();
    if(state == 1) {
        cout << "\n\n1 (SAFE STATE)" << endl;
        //cout << "All processes can execute without deadlocks." << endl;
    } else {
        cout << "\n\n0 (UNSAFE STATE)" << endl;
    }
}
//----------------calculates needs of resources---------
void calculateneed();
int main()
{
    process *temp;
    resource *tempres;
    initialize();
    calculateneed();
    temp=plist;
    cout<<"\nMax list\t\t"<<"Allocated list \t\t"<<"Need list";
    while(temp!=NULL)
    {
        cout<<endl;
        tempres=temp->maxlist;
        while(tempres!=NULL){
            cout<<tempres->amount<<" ";
            tempres=tempres->next;
        }
        tempres=temp->allocatedlist;
        cout<<"\t\t\t";
        while(tempres!=NULL){
            cout<<tempres->amount<<" ";
            tempres=tempres->next;
        }
        //need output
        tempres=temp->needlist;
        cout<<"\t\t\t";
        while(tempres!=NULL){
            cout<<tempres->amount<<" ";
            tempres=tempres->next;
        }       
        temp=temp->next;
    }
    // new additions executed here 
    calculatefreeresources();  // generates the freelist
    safechecker();             // evaluates if it is safe (1) or unsafe (0)
    return 0;
}
//------------------------------
//----------------------------------------
//-----------initialize definition
//-------------initialize definition
void calculateneed(){
    process *temp;
    resource *tempr;
    resource *maxptr;
    resource *allocateptr;
    temp = plist;
    while(temp != NULL){
        maxptr= temp->maxlist;
        allocateptr=temp->allocatedlist;
        while(maxptr!=NULL){
            tempr = new resource;
            tempr->next=NULL;
            tempr->amount= maxptr->amount - allocateptr->amount;
            addlasttoresourcelist(tempr);
            maxptr=maxptr->next;
            allocateptr=allocateptr->next;
        }
        temp->needlist=tempresourcelist;
        tempresourcelist=NULL;
        temp= temp->next;
    }
}
//initialize definition
void initialize()
{
    int n,m;
    process *temp;
    resource *tempr,*totalinstance, *currentallocatedresource;
    cout<<"\nEnter the total number of processes---";
    cin>>n;
    cout<<"\nAnd now enter the total number of resource types--";
    cin>>m;

    for(int j=1;j<=m;j++)
    {
        tempr=new resource;
        tempr-> next=NULL;
        cout<<"\nEnter total number of instances for resource type: "<<j<<": ";
        cin>>tempr->amount;
        addlasttoresourcelist(tempr);
    }// end loop j
    cout<<"\nfiniiiished";
    totallist=tempresourcelist;
    tempresourcelist=NULL;
    cout<<"\n\n\nEnter the max list for all processes: "<<endl;
    for(int i=1;i<=n;i++)
    {
        totalinstance=totallist;
        cout<<"\nMax list for process "<<i<<": ";
        temp=new process;
        temp->pid=i;
        temp->next=NULL;
        for(int k=1;k<=m;k++) 
        {
            cout<<"\n\tFor resource type "<<k<<": ";
            tempr=new resource;
            tempr->rid=k;
            tempr->next=NULL;
            max:cin>>tempr->amount;         
            if(tempr->amount>totalinstance->amount)
            {
                cout<<"\nThere is no such resource. Please enter a number <= "<<totalinstance->amount;
                goto max;  
            } //end if
            addlasttoresourcelist(tempr);
            totalinstance=totalinstance->next; 
        }//end loop k
        temp->maxlist=tempresourcelist;
        tempresourcelist=NULL;
        addlastprocess(temp);
    }//end loop i
    cout<<"\n\n\n\t Now enter the allocated list for each process\n";
    temp=NULL;
    temp=plist;
    for(int i=1;i<=n;i++)
    {
        currentallocatedresource=temp->maxlist;
        cout<<"\nEnter allocated list for process "<<i<<": ";
        
        for(int k=1;k<=m;k++)
        {
            tempr=new resource;
            tempr->next=NULL;
            tempr->rid=k;
            cout<<"\n\tresource type "<<k<<": ";
            allcted:cin>>tempr->amount;
            if(tempr->amount>currentallocatedresource->amount)
                goto allcted;           
            addlasttoresourcelist(tempr);
            currentallocatedresource=currentallocatedresource->next; 
        }
        temp->allocatedlist=tempresourcelist;
        tempresourcelist=NULL;
        temp=temp->next;
    }
}
