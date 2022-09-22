#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "lcgrand.h"  /* Header file for random-number generator. */
#include "lcgrand.c"

#define Q_LIMIT_1 100  /* Limit on queue length1 */
#define Q_LIMIT_2 10     /* Limit on queue length2 */
#define Q_LIMIT_3 20    /* Limit on queue length3 */
#define Q_LIMIT_4 30    /* Limit on queue length4 */


#define BUSY      1  /* Mnemonics for server's being busy */
#define IDLE      0  /* and idle. */


#define SEED 1


int next_event_type, num_events, q, aux_apt;
int num_cust_delayed[5];
int num_in_q[5];
int server_status[5];
int q_limit[] = {0, Q_LIMIT_1, Q_LIMIT_2, Q_LIMIT_3, Q_LIMIT_4};

float area_num_in_q[5];
float area_server_status[5];
float var_arrival, sim_time, time_max;
float time_last_event[5];
float time_next_event[9];
float mean_service[5];
float total_delay[5];
float probability[] = {0, 0, 0.2, 0.6, 1.0};

float time_arrival_1[Q_LIMIT_1+1], time_arrival_2[Q_LIMIT_2+1], time_arrival_3[Q_LIMIT_3+1], time_arrival_4[Q_LIMIT_4+1];



FILE  *infile, *outfile;


void  initialize(void);
void  timing(void);
void  arrive(int q);
void  depart(int q);
void  report(void);
void  update_time_avg_stats(int q);
float expon(float mean);
float poiss(float var);

//num events = 9


main()
{
    infile = fopen("1-9.in", "r");
    outfile = fopen("1-9.out", "w");
    int contador = 0;

    num_events = 8;

    fscanf(infile, "%f %f %f %f %f %f", &mean_service[1], &mean_service[2], &mean_service[3], &mean_service[4], &var_arrival, &time_max);

    fprintf(outfile, "Punto 1.9 / Tarea 1\n\n");
    fprintf(outfile, "Mean service 1 %f\n\n",mean_service[1]);
    fprintf(outfile, "Mean service 2 %f\n\n",mean_service[2]);
    fprintf(outfile, "Mean service 3 %f\n\n",mean_service[3]);
    fprintf(outfile, "Mean service 4 %f\n\n",mean_service[4]);
    fprintf(outfile, "Lambda arrival %f\n\n", var_arrival);

    initialize();

    while(sim_time<time_max*24*60)
    {
        contador++;
        for(int i = 1; i<5; i++)
        {
        }
        timing();

        switch(aux_apt)
        {
        case 1:
            arrive(1);
            update_time_avg_stats(1);
            break;
        case 2:
            depart(1);
            update_time_avg_stats(1);
            break;
        case 3:
            arrive(2);
            update_time_avg_stats(2);
            break;
        case 4:
            depart(2);
            update_time_avg_stats(2);
            break;
        case 5:
            arrive(3);
            update_time_avg_stats(3);
            break;
        case 6:
            depart(3);
            update_time_avg_stats(3);
            break;
        case 7:
            arrive(4);
            update_time_avg_stats(4);
            break;
        case 8:
            depart(4);
            update_time_avg_stats(4);
            break;
        default:
            break;

        }
    }

    report();
    printf("\n\n%d\n\n",contador);


    fclose(infile);
    fclose(outfile);

    return 0;
}



void initialize(void)
{
    sim_time = 0.0;

    // Set all server status to IDLE
    for(int i = 0; i<5; i++ )
    {
        server_status[i] = IDLE;
    }

    // Set all queues to 0

    for(int i = 0; i<5;i++)
    {
        num_in_q[i] = 0;
    }

    //set last event on queues to 0
    for(int i = 0; i<5; i++)
    {
        time_last_event[i] = 0.0;
    }

    //Initialize statistical counters
    for(int i = 0; i<5; i++)
    {
        num_cust_delayed[i] = 0;
    }

    for(int i = 0; i<5; i++)
    {
        total_delay[i] = 0;
    }

    for(int i = 0; i<5; i++)
    {
        area_num_in_q[i] = 0;
    }

    for(int i = 0; i<5; i++)
    {
        area_server_status[i] = 0;
    }



    //initialize event list.
    //schedule a new arrival at 1.
    time_next_event[1] = sim_time + poiss(var_arrival);
    q = 1;
    aux_apt = 1;
    for(int i = 2; i<9; i++)
    {
        time_next_event[i] = 1.0e+30;
    }

}


void timing(void)
{

    float min_time_next_event = 1.0e+29;

    next_event_type = 0;

    //determine the next event to happen

    for(int i = 1; i<=num_events; i++)
    {
        if(time_next_event[i] < min_time_next_event)
        {
            min_time_next_event = time_next_event[i];
            next_event_type = i;
            aux_apt = i;
        }
    }

    //check if event list is empty

    if(next_event_type == 0)
    {
        fprintf(outfile, "\nEvent list empty at time %f", sim_time);
        exit(1);
    }

    sim_time = min_time_next_event;
}


void  arrive(int q) /* Function that declare arrival */
{
    //if arrive to 1, schedule next arrival to 1.
    if(q==1)
    {
        time_next_event[1] = sim_time + poiss(var_arrival);
    }

    float delay;

    if(server_status[q] == BUSY) //Nq is busy
    {
        (num_in_q[q])++; // Add 1 to queue q.

        if(num_in_q[q] > q_limit[q]) //Limit of the Queue N exceed
        {
            fprintf(outfile, "\nQueue limit exceeded on N");
            fprintf(outfile, " N %d", q);
            exit(2);
        }

        switch(q)
        {
        case 1:
            time_arrival_1[num_in_q[q]] = sim_time;
            break;
        case 2:
            time_arrival_2[num_in_q[q]] = sim_time;
            break;
        case 3:
            time_arrival_3[num_in_q[q]] = sim_time;
            break;
        case 4:
            time_arrival_4[num_in_q[q]] = sim_time;
            break;
        default:
            printf("not working arrive, queue not in range");
            break;

        }

    }

    else // N is idle
    {
        delay = 0.0;
        (total_delay[q]) +=delay;

        (num_cust_delayed[q])++;
        server_status[q] = BUSY;
        int aux_ind = q*2;
        time_next_event[aux_ind] = sim_time+expon(mean_service[q]);


    }




}


void depart(int q)
{

    int i;
    float delay;
    //Queue is empty, change state and reset the depart on list
    if(num_in_q[q] == 0)
    {
        server_status[q] = IDLE;
        int aux_i = q*2;
        time_next_event[aux_i] = 1.0e+30;
    }
    else
    {
        //put one out of queue
        (num_in_q[q])--;

        //compute delay
        switch(q)
        {
        case 1:
            delay = sim_time - time_arrival_1[1];
            break;
        case 2:
            delay = sim_time - time_arrival_2[1];
            break;
        case 3:
            delay = sim_time - time_arrival_3[1];
            break;
        case 4:
            delay = sim_time - time_arrival_4[1];
            break;
        default:
            printf("not working departure, queue not in range");
            break;

        }
        //plus 1 to number of customer delay on the Queue
        (total_delay[q]) +=delay;
        (num_cust_delayed[q])++;

        //move customer in queue up one place

        for(i = 1; i<=num_in_q[q]; i++)
        {
            switch(q)
            {
            case 1:
                time_arrival_1[i] = time_arrival_1[i+1];
                break;
            case 2:
                time_arrival_2[i] = time_arrival_2[i+1];
                break;
            case 3:
                time_arrival_3[i] = time_arrival_3[i+1];
                break;
            case 4:
                time_arrival_4[i] = time_arrival_4[i+1];
                break;
            default:
                printf("Not working departure, customers not moving 1");

            }
        }



    }

    //Send the thing to the next Queue

    //if the queue is 1, send to 2,3 or 4 following probability
    if(q == 1)
    {
        int arrival_queue = 0;
        float rand_aux = lcgrand(SEED);
        if(rand_aux<=probability[2])
        {
            arrival_queue = 2;
        }
        else if(rand_aux>probability[2] && rand_aux<= probability[3])
        {
            arrival_queue = 3;
        }
        else
        {
            arrival_queue = 4;
        }

        arrive(arrival_queue);


    }
    else //if not one, send to the next, if q ==4, don't arrive
    {
        int next_q = q+1;
        if(next_q<=4)
        {
            arrive(next_q);
        }
    }

}



void update_time_avg_stats(int q)
{
    float time_since_last_event;

    //compute time since last event, update time per Queue
    time_since_last_event = sim_time-time_last_event[q];
    time_last_event[q] = sim_time;

    //Update area under number-in-queue function

    area_num_in_q[q]+= num_in_q[q]*time_since_last_event;

    //Update area under server-busy function
    area_server_status[q] += server_status[q] * time_since_last_event;


}



void report()
{
    //declare total estimates
    float total_number_of_delay = 0;
    int total_customer_delayed = 0;

    float total_area_num_in_q = 0;
    float total_area_server_status = 0;

    //Compute and write individual estimates

    fprintf(outfile, "\n\nAverage delay in single queue\n\n");
    for(int i = 1; i<5; i++)
    {
        total_number_of_delay+= total_delay[i];
        total_customer_delayed+=num_cust_delayed[i];
        fprintf(outfile, "\n\nThe average delay in queue %d is %f minutes", i, total_delay[i]/num_cust_delayed[i]);
    }
    fprintf(outfile, "\n\nThe average delay in all queues is %f minutes", total_number_of_delay/total_customer_delayed);

    fprintf(outfile, "\n\nAverage number in queue\n\n");
    for(int i = 1; i<5; i++)
    {
        total_area_num_in_q += area_num_in_q[i];
        float a = area_num_in_q[i];
        float b = sim_time/1440;
        fprintf(outfile, "\n\nThe average number on queue %d is %f", i, a/b);
    }
    float aux = sim_time/1440;
    fprintf(outfile, "\n\nThe average number in all queues is %f", total_area_num_in_q/aux);

    fprintf(outfile, "\n\nAverage server utilization at queue\n\n");
    for(int i = 1; i<5; i++)
    {
        total_area_server_status += area_server_status[i];
        float a = area_server_status[i];
        float b = sim_time/60;
        fprintf(outfile, "\n\nThe server utilization in queue %d is %f", i, a/b);
    }
    fprintf(outfile, "\n\nThe average server status in all queues is %f", total_area_server_status/(sim_time/60));

    fprintf(outfile, "\n\nTime simulation ended %12.3f days", sim_time/(60));

}



float expon(float mean)  /* Exponential variate generation function. */
{
    float aux = -mean * log(lcgrand(SEED));
    /* Return an exponential random variate with mean "mean". */

    return aux;
}

float poiss(float var)
{
    float m = 1/var;
    float aux = expon(m);
    return aux;

}



