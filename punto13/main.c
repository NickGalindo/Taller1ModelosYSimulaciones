#include <stdio.h>
#include <stdlib.h>
#include "lcgrand.h"
#include <math.h>
#define End_day 480 /*Finalizacion de la simulacion*/
#define Q_LIMIT 100 /*Tamaño máximo colas*/
#define BUSY 1 /*Taquilla ocupada*/
#define IDLE 0 /*Taquilla libre*/

int   next_event_type, num_custs_delayed_q, num_custs_delayed_c, num_events,
      num_in_q, num_in_c, server_status;

float area_num_in_q,area_num_in_c, area_server_status, mean_interarrival_q,mean_interarrival_c,
      mean_service_q,mean_service_c,sim_time, time_arrival_q[Q_LIMIT + 1],time_arrival_c[Q_LIMIT + 1]
      , time_last_event, time_next_event[5], total_delay_q, total_delay_c;

void  initialize(void);
void  timing(void);
void  arrive_q(void);
void  arrive_c(void);
void  depart_q(void);
void  depart_c(void);
void  report(void);
void  update_time_avg_stats(void);
float expon(float mean);



int main()
{
    mean_interarrival_q=12.0;
    mean_service_q=6.0;
    mean_interarrival_c=10.0;
    mean_service_c=5.0;

    num_events=4;

    initialize();

    while (sim_time <= End_day)
    {
        /* Determina el siguiente evento */

        timing();

        /* Actualiza contadores y estadisticas */

        update_time_avg_stats();

        /* Invoca el evento */

        /*
        printf("\nev:%d ",next_event_type);
        printf("\qu:%d ",num_in_q);
        printf("-ca:%d ",num_in_c);
        printf("-BU:%d",server_status);
        printf("-sim:%f",sim_time);
        */

        switch (next_event_type)
        {
            case 1:
                arrive_q();
                break;
            case 2:
                arrive_c();
                break;
            case 3:
                depart_q();
                break;
            case 4:
                depart_c();
                break;
        }
    }
    report();
    return 0;
}

void initialize(){
    sim_time=0.0;

     /* Inicializa variables de estado */

    server_status = IDLE;
    num_in_q = 0;
    num_in_c = 0;
    time_last_event = 0.0;

    /* Inicializa contadores estadisticos */

    num_custs_delayed_q = 0;
    num_custs_delayed_c = 0;
    total_delay_q =0.0;
    total_delay_c =0.0;
    area_num_in_q = 0.0;
    area_num_in_c = 0.0;
    area_server_status = 0.0;

    /* Establecer eventos iniciales*/

    time_next_event[1] = sim_time + 2.0;
    time_next_event[2] = sim_time + 3.0;
    time_next_event[3] = 1.0e+30;
    time_next_event[4] = 1.0e+30;

}

void timing(void)  /* Timing */
{
    int   i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0;

    /* Determina el siguiente evento*/

    for (i = 1; i <= num_events; ++i)
        if (time_next_event[i] < min_time_next_event)
        {
            min_time_next_event = time_next_event[i];
            next_event_type     = i;
        }

    /* Verifica si no hay eventos */

    if (next_event_type == 0)
    {
        printf("\nEvent list empty at time %f", sim_time);
        exit(1);
    }


    /* Avanza el reloj de la simulacion*/

    sim_time = min_time_next_event;
}


void arrive_q(void)  /* Llegada a la cola presencial*/
{
    float delay;

    /* Programar proxima llegada */


    /*printf("\n%11.3f",sim_time);*/
    time_next_event[1] = sim_time + expon(mean_interarrival_q);

    /* Verifica taquilla ocupada */

    if (server_status == BUSY)
    {

        ++num_in_q;

        if (num_in_q > Q_LIMIT)
        {
            printf("\nOverflow of the array time_arrival at");
            printf(" time %f", sim_time);
            exit(2);
        }

        time_arrival_q[num_in_q] = sim_time;
    }else
    {
        /* Taqulla libre */

        delay = 0.0;
        total_delay_q += delay;

        /* Aumenta numero de clientes atendidos presencialmente y actualiza estado de la taquilla*/

        ++num_custs_delayed_q;
        server_status = BUSY;

        /* Programa partida */

        time_next_event[3] = sim_time + expon(mean_service_q);
    }

}

void arrive_c(void)  /* Llegada de una llamada */
{
    float delay;

    /* Programar proxima llegada */

    time_next_event[2] = sim_time + expon(mean_interarrival_c);

    /* Verifica taquilla ocupada */

    if (server_status == BUSY)
    {

        ++num_in_c;

        if (num_in_c > Q_LIMIT)
        {
            printf("\nOverflow of the array time_arrival at");
            printf(" time %f", sim_time);
            exit(2);
        }

        time_arrival_c[num_in_c] = sim_time;
    }else
    {
        /* Taqulla libre */

        delay = 0.0;
        total_delay_c += delay;

        /* Aumenta numero de clientes atendidos por llamada y actualiza estado de la taquilla*/

        ++num_custs_delayed_c;
        server_status = BUSY;

        /* Programa partida */

        time_next_event[4] = sim_time + expon(mean_service_c);

    }
}

void depart_q(void)  /* Partida de cliente en la cola*/
{
    int   i;
    float delay;

    /* Verificar si la cola presencial está vacía*/

    if (num_in_q == 0)
    {

        server_status      = IDLE;
        time_next_event[3] = 1.0e+30;
    }else
    {
        /* Reduce los clientes en la cola */

        --num_in_q;

        /* Actualizar tiempo de espera de la cola presencial */

        delay= sim_time - time_arrival_q[1];
        total_delay_q += delay;

        /* Aumenta numero de clientes atendidos presencialmente y programa partida */

        ++num_custs_delayed_q;
        time_next_event[3] = sim_time + expon(mean_service_q);

        /* Mueve los clientes de la cola */

        for (i = 1; i <= num_in_q; ++i)
            time_arrival_q[i] = time_arrival_q[i + 1];
    }
}

void depart_c(void)  /* Partida de clientes en llamada*/
{
    int   i;
    float delay;

    /* Verificar si la cola de llamada está vacía */

    if (num_in_c == 0)
    {

        server_status      = IDLE;
        time_next_event[4] = 1.0e+30;
    }
    /* Atiende clientes en llamada solo cuando no hay clientes en la cola presencial*/
    else if (num_in_q==0){
        /* Reduce los clientes en la cola*/

        --num_in_c;

        /* Actualizar tiempo de espera de la cola de llamadas */

        delay= sim_time - time_arrival_c[1];
        total_delay_c += delay;

        /* Aumenta numero de clientes atendidos en llamada y programa partida */

        ++num_custs_delayed_c;
        time_next_event[4] = sim_time + expon(mean_service_c);

        /* Move each customer in queue (if any) up one place. */

        for (i = 1; i <= num_in_c; ++i)
            time_arrival_c[i] = time_arrival_c[i + 1];
    }
    /*En caso de haber clientes en la cola presencial, marca la taquilla ocupada*/
    else{
        server_status      = IDLE;
        time_next_event[4] = 1.0e+30;
    }

}

void report(void)  /* Report generator function. */
{
    /* Compute and write estimates of desired measures of performance. */


    printf("Average delay in queue%11.3f minutes\n\n",total_delay_q / num_custs_delayed_q);

    printf("Average delay in call%11.3f minutes\n\n",total_delay_c / num_custs_delayed_c);

    printf("Average number in queue%10.3f minutes\n\n",area_num_in_q / sim_time);

    printf("Average number in call%10.3f minutes\n\n",area_num_in_c / sim_time);

    printf("Server utilization%15.3f minutes\n\n",area_server_status / sim_time);

    printf("Time simulation ended%12.3f minutes\n\n", sim_time);

    printf("Total num delayed queue %d clientes\n\n", num_custs_delayed_q);

    printf("Total num delayed call %d clientes\n\n", num_custs_delayed_c);
}


void update_time_avg_stats(void)  /* Actualiza acumuladores estadisticos*/
{
    float time_since_last_event;

    /* Actualiza el tiempo del ultimo evento */

    time_since_last_event = sim_time - time_last_event;
    time_last_event = sim_time;

    /* Actualiza area dado clientes en las colas */

    area_num_in_q += num_in_q * time_since_last_event;
    area_num_in_c += num_in_c * time_since_last_event;

    /* Actualiza area del estado de la taquilla */

    area_server_status += server_status * time_since_last_event;
}


float expon(float mean)  /* Exponential variate generation function. */
{
    /* Return an exponential random variate with mean "mean". */

    return -mean * log(lcgrand(1));
}
