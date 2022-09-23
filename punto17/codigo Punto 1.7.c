#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Q_LIMIT  1000
#define BUSY  1 /* Mnemonics for server's being busy */
#define IDLE    0 /* and idle. */
#define PI  3.14159265358979323846

int next_event, num_events, clients_in_q, employee_status,
total_clients, cost_A, cost_B, sales_A, sales_B,
util_A, util_B, util_total,
caso_a,
caso_b,
caso_c,
numero;


float sim_time, //simulation clock
media_llegada, //3/min
media_atencion_a1, //uniforme 3.1 y 3.8 min
media_atencion_a2, //uniforme 3.1 y 3.8 min
media_atencion_b, //exponencial media 7 min
media_atencion_c, //1.5 min
fin_simulacion, //8 horas
tiempo_total_fila_empleado,
tiempo_total_fila_reparador,
area_cola,
area_atencion,
tiempo_ultimo_evento,
total_demora,
tiempo_llegada[1000], //colas
time_next_event[3];

FILE *infile, *outfile;

void initialize(void);
void arrival(void);
void timing(void);
void report(void);
void depart(void);
void update_time_avg_status(void);

float expon(float mean, int stream);
int   random_integer(float prob_distrib[], int stream);
float uniform(float a, float b, int stream);
float lcgrand(int stream);
void  lcgrandst(long zset, int stream);
long  lcgrandgt(int stream);
float normal(float media, float varianza, int stream1, int stream2);

int main()
{
    infile = fopen("Entry-6.txt", "r");
    outfile = fopen("Depart-6.txt", "w");

    num_events = 3;

    fscanf(infile, "%f %f %f %f %f %i %i", &media_llegada, &media_atencion_a1, &media_atencion_b,
           &media_atencion_c, &fin_simulacion, &cost_A, &cost_B);

    printf(outfile, "    ----    Punto 1-6    ----    \n\n");
    fprintf(outfile, "Media llegada de clientes: %1.0f / hora.\n", total_clients/8);
    fprintf(outfile, "Media atención para el caso A %1.0f minutos.\n", media_atencion_a1);
    fprintf(outfile, "Media atención para el caso B %1.0f minutos.\n", media_atencion_b);
    fprintf(outfile, "Media atención para el caso C %1.0f minutos.\n", media_atencion_c);
    fprintf(outfile, "Cobro por prenda A %1.0i\n", cost_A);
    fprintf(outfile, "Cobro por prenda B %1.0i\n", cost_B);
    fprintf(outfile, "\nDuración de la simulación: %1.0f minutos\n", fin_simulacion);

    /* Initialize the simulation. */
    initialize();
    while (sim_time <= fin_simulacion) {
        timing();
        switch(next_event){
            case 1:
                arrival();
                break;
            case 2:
                depart();
                break;
        }
    }

    report();

    fclose(infile);
    fclose(outfile);

    return 0;
}

void initialize(void)
{
    sim_time = 0.0;
    employee_status = IDLE; //busy 1, idle 0
    clients_in_q = 0;
    area_atencion = 0;

    sales_A = 0;
    sales_B = 0;
    util_A = 0;
    util_B = 0;
    tiempo_ultimo_evento = 0;
    total_demora = 0;

    area_cola = 0;

    tiempo_total_fila_empleado = 0;
    time_next_event[1] = sim_time + expon(media_llegada, total_clients);
    time_next_event[2] = 1.0e+30;

}

void timing(void)
{
    int i;
    float min_time_next_event = 1.0e+29;
    next_event = 0;

    for (i=1; i<=num_events; ++i) {
        if (time_next_event[i] < min_time_next_event) {
            min_time_next_event = time_next_event[i];
            next_event=i;
        }
    }

    if (next_event == 0)
    {
        fprintf(outfile, "\nEvent list empty at time %f", sim_time);
        exit(1);
    }
    sim_time = min_time_next_event;
}

void arrival(void)
{
    float demora;
    time_next_event[1] = sim_time + expon(media_llegada, total_clients);

    if(employee_status==IDLE)
    {
        demora = 0;
        employee_status = BUSY;

        numero = rand() % (10);

        if(numero==0 || numero==1)
        {
            time_next_event[2] = sim_time + media_atencion_c;
            tiempo_total_fila_reparador = time_next_event[2]-sim_time;
        }

        if(numero==2 || numero==3 || numero==4 || numero==5 || numero==6)
        {
            time_next_event[2] = sim_time + uniform(media_atencion_a1, media_atencion_a2, total_clients);
            tiempo_total_fila_reparador = time_next_event[2]-sim_time;
        }

        if(numero==7 || numero==8 || numero==9)
        {
            time_next_event[2] = sim_time + expon(media_atencion_b, total_clients);
            tiempo_total_fila_reparador = time_next_event[2]-sim_time;
        }

    }
    else
    {
        ++clients_in_q;
        if (clients_in_q > Q_LIMIT) {
        /* overflowed queue, stop the simulation. */
            fprintf(outfile, "\nOverflow of the array time_arrival at");
            (outfile, " time %f", sim_time);
            exit(2);
        }
        tiempo_llegada[clients_in_q]=sim_time;
    }
}


void depart(void)
{
    int i;
    float demora;

        if(numero==2 || numero==3 || numero==4 || numero==5 || numero==6) {
            sales_A++;
        }

        if(numero==7 || numero==8 || numero==9) {
            sales_B++;
        }

    if(clients_in_q == 0) {
        employee_status = IDLE;
        time_next_event[2] = 1.0e+29;
    }

    else {
        --clients_in_q;
        demora = sim_time - tiempo_llegada[1];

        total_demora += demora;

        if(numero==0 || numero==1)
        {
            time_next_event[2] = sim_time + media_atencion_c;
            tiempo_total_fila_empleado = time_next_event[2] - tiempo_llegada[1];

            for (i=1; i<=clients_in_q; i++)
            {
                tiempo_llegada[i] = tiempo_llegada[i+1];
            }
        }

        if(numero==2 || numero==3 || numero==4 || numero==5 || numero==6)
        {
            time_next_event[2] = sim_time + uniform(media_atencion_a1, media_atencion_a2, total_clients);
            tiempo_total_fila_empleado = time_next_event[2] - tiempo_llegada[1];

            for (i=1; i<=clients_in_q; i++)
            {
                tiempo_llegada[i] = tiempo_llegada[i+1];
            }
        }

        if(numero==7 || numero==8 || numero==9)
        {
            time_next_event[2] = sim_time + expon(media_atencion_b, total_clients);
            tiempo_total_fila_empleado = time_next_event[2] - tiempo_llegada[1];

            for (i=1; i<=clients_in_q; ++i)
            {
                tiempo_llegada[i] = tiempo_llegada[i+1];
            }
        }
    }
}

void report(void)
{

    int util_A = sales_A *2500;
    int util_B = sales_B *4500;
    int util_total = util_A + util_B;

    printf("%i", 24/8);

    fprintf(outfile, "\n\n    ----    Punto 1-6. Resultados    ----    \n\n");
    fprintf(outfile, "Clientes totales: %i \n", total_clients);
    /* Análisis */
    fprintf(outfile, "\nHora en que el empleado termina turno: \n", sim_time_empl); //
    fprintf(outfile, "\nValor estimado utilidades diarias: \n", util_total);
    fprintf(outfile, "\nTiempo promedio espera cliente para ser atendido: \n", tiempo_total_fila_empleado);
    fprintf(outfile, "\nTiempo entre llegadas: \n", media_llegada);
    fprintf(outfile, "\nNúmero de empleados: \n", num_employ);
    fprintf(outfile, "\nNúmero de clientes en cola: \n", num_clients); //

}

float expon(float mean, int stream)
{
    return -mean * log(lcgrand(stream));

}