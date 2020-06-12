#include "stdafx.h"
#include "QueueingSolvers.h"
#include <numeric>
#include <algorithm>
#define SEED_ARRAY_LENGTH 101
#include "RandomStream.h"

QueueingClass::QueueingClass(){
}
QueueingClass::~QueueingClass(){
}
FixedCustomerQueueing::FixedCustomerQueueing(){
}
FixedLengthQueueing::FixedLengthQueueing(){
}
FixedCustomerQueueing::~FixedCustomerQueueing(){
}
FixedLengthQueueing::~FixedLengthQueueing(){
}

LimitCapacityQueueing::LimitCapacityQueueing(){
}
LimitCapacityQueueing::~LimitCapacityQueueing(){}

LimitOpenTimeQueueing::LimitOpenTimeQueueing(){}
LimitOpenTimeQueueing::~LimitOpenTimeQueueing(){}

float QueueingClass::expon(float mean,int seed){
	return -mean * log(RandomGenerator::lcgrand(seed));
}

void QueueingClass::initialize(int seed)  /* Initialization function. */
{
    /* Initialize the simulation clock. */
	num_events = 2;

    sim_time = 0.0;

    /* Initialize the state variables. */

    server_status   = IDLE;
    num_in_q        = 0;
	num_in_system   = 0;
    time_last_event = 0.0;

    /* Initialize the statistical counters. */

    num_custs_delayed  = 0;
	num_delayed_excess = 0;
    total_of_delays    = 0.0;
	total_time_in_system =0.0;
    area_num_in_q      = 0.0;
	area_num_in_system = 0.0;
    area_server_status = 0.0;
	max_num_in_q       = 0;
	max_delay_in_q     = 0.0;
	max_time_in_system = 0.0;
	num_custs_served   = 0;
	num_of_balk        = 0;

    /* Initialize event list.  Since no customers are present, the departure
       (service completion) event is eliminated from consideration. */

    
	time_next_event[1] = sim_time + expon(mean_interarrival,seed);
    time_next_event[2] = 1.0e+30;
}

void QueueingClass::timing(void)  /* Timing function. */
{
    int   i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0;

    /* Determine the event type of the next event to occur. */

    for (i = 1; i <= num_events; ++i)
        if (time_next_event[i] < min_time_next_event)
        {   
            min_time_next_event = time_next_event[i];
            next_event_type     = i;
        }

    /* Check to see whether the event list is empty. */

    if (next_event_type == 0)
    {
        /* The event list is empty, so stop the simulation. */

       /* fprintf(outfile, "\nEvent list empty at time %f", sim_time);*/
        exit(1);
    }

    /* The event list is not empty, so advance the simulation clock. */

    sim_time = min_time_next_event;
}


void QueueingClass::timing3(void)  /* Timing function. */
{
    int   i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0;

    /* Determine the event type of the next event to occur. */

    for (i = 1; i <= num_events; ++i)
        if (time_next_event[i] < min_time_next_event)
        {   
            min_time_next_event = time_next_event[i];
            next_event_type     = i;
        }

    /* Check to see whether the event list is empty. */

    if (next_event_type == 0)
    {
        /* The event list is empty, so stop the simulation. */

       /* fprintf(outfile, "\nEvent list empty at time %f", sim_time);*/
        exit(1);
    }

	
    /* The event list is not empty, so advance the simulation clock. */

    sim_time = min_time_next_event;

	if(sim_time>480)
	{
		next_event_type=3;
		sim_time=480;
	}
}


void QueueingClass::arrive(int seed)  /* Arrival event function. */
{
    float delay;

	++num_in_system;

    /* Schedule next arrival. */

    time_next_event[1] = sim_time + expon(mean_interarrival,seed);

    /* Check to see whether server is busy. */

    if (server_status == BUSY)
    {
        /* Server is busy, so increment number of customers in queue. */

        ++num_in_q;

		if(num_in_q>max_num_in_q)
		{
			max_num_in_q=num_in_q;
		}

        /* Check to see whether an overflow condition exists. */

        if (num_in_q > Q_LIMIT)
        {
            /* The queue has overflowed, so stop the simulation. */

            /*fprintf(outfile, "\nOverflow of the array time_arrival at");
            fprintf(outfile, " time %f", sim_time);*/
            exit(2);
        }

        /* There is still room in the queue, so store the time of arrival of the
           arriving customer at the (new) end of time_arrival. */

        time_arrival[num_in_q] = sim_time;

    }

    else
    {
        /* Server is idle, so arriving customer has a delay of zero.  (The
           following two statements are for program clarity and do not affect
           the results of the simulation.) */

        delay            = 0.0;
        total_of_delays += delay;

		if(delay>excess_delay)
		{
			num_delayed_excess++;
		}

        /* Increment the number of customers delayed, and make server busy. */

        ++num_custs_delayed;
        server_status = BUSY;

        /* Schedule a departure (service completion). */

        time_next_event[2] = sim_time + expon(mean_service,seed);

		/*time_in_system=time_next_event[2]-sim_time;
		total_time_in_system += time_in_system;

		if(time_in_system>max_time_in_system)
		{
			max_time_in_system=time_in_system;
		}*/

		time_arrival[num_in_q] = sim_time;
    }
}


void QueueingClass::depart(int seed)  /* Departure event function. */
{
    int   i;
    float delay;

	--num_in_system;

    /* Check to see whether the queue is empty. */

    if (num_in_q == 0)
    {
        /* The queue is empty so make the server idle and eliminate the
           departure (service completion) event from consideration. */

        server_status      = IDLE;
        time_next_event[2] = 1.0e+30;

    }

    else
    {
        /* The queue is nonempty, so decrement the number of customers in
           queue. */

        --num_in_q;

        /* Compute the delay of the customer who is beginning service and update
           the total delay accumulator. */

        delay            = sim_time - time_arrival[1];
        total_of_delays += delay;

		if(delay>max_delay_in_q)
		{
			max_delay_in_q=delay;
		}

		if(delay>excess_delay)
		{
			num_delayed_excess++;
		}

        /* Increment the number of customers delayed, and schedule departure. */

        ++num_custs_delayed;
        time_next_event[2] = sim_time + expon(mean_service,seed);

		/*time_in_system = time_next_event[2] - time_arrival[1];
		total_time_in_system += time_in_system;*/

		

        /* Move each customer in queue (if any) up one place. */

    }

	

	++num_custs_served;

	time_in_system = sim_time - time_arrival[0];
	    total_time_in_system += time_in_system;

	if(time_in_system>max_time_in_system)
	{
		max_time_in_system=time_in_system;
	}

	for (i = 0; i <= num_in_q; ++i)
            time_arrival[i] = time_arrival[i + 1];

	
}


void QueueingClass::update_time_avg_stats(void)  /* Update area accumulators for time-average
                                     statistics. */
{
    float time_since_last_event;

    /* Compute time since last event, and update last-event-time marker. */

    time_since_last_event = sim_time - time_last_event;
    time_last_event       = sim_time;

    /* Update area under number-in-queue function. */

    area_num_in_q      += num_in_q * time_since_last_event;

	num_in_system       = num_in_q+server_status;
	area_num_in_system += num_in_system * time_since_last_event;

    /* Update area under server-busy indicator function. */

    area_server_status += server_status * time_since_last_event;
}


void QueueingClass::timing2(void)  /* Timing function. */
{
    int   i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0;

    /* Determine the event type of the next event to occur. */

    for (i = 2; i <= num_events; ++i)
        if (time_next_event[i] < min_time_next_event)
        {   
            min_time_next_event = time_next_event[i];
            next_event_type     = i;
        }

    /* Check to see whether the event list is empty. */

    if (next_event_type == 0)
    {
        /* The event list is empty, so stop the simulation. */

       /* fprintf(outfile, "\nEvent list empty at time %f", sim_time);*/
        exit(1);
    }

    /* The event list is not empty, so advance the simulation clock. */

    sim_time = min_time_next_event;
}

void QueueingClass::arrive2(int seed)  /* Arrival event function. */
{
    float delay;

    /* Schedule next arrival. */

    time_next_event[1] = sim_time + expon(mean_interarrival,seed);

    /* Check to see whether server is busy. */

    if ((server_status == BUSY)&&(num_in_q<queue_capacity))
    {
        /* Server is busy, so increment number of customers in queue. */

        ++num_in_q;
		++num_in_system;

		if(num_in_q>max_num_in_q)
		{
			max_num_in_q=num_in_q;
		}

        /* Check to see whether an overflow condition exists. */

        if (num_in_q > Q_LIMIT)
        {
            /* The queue has overflowed, so stop the simulation. */

            /*fprintf(outfile, "\nOverflow of the array time_arrival at");
            fprintf(outfile, " time %f", sim_time);*/
            exit(2);
        }

        /* There is still room in the queue, so store the time of arrival of the
           arriving customer at the (new) end of time_arrival. */

        time_arrival[num_in_q] = sim_time;

    }
	else if((server_status == BUSY)&&(num_in_q>=queue_capacity))
	{
		num_of_balk++;
	}

    else
    {
        /* Server is idle, so arriving customer has a delay of zero.  (The
           following two statements are for program clarity and do not affect
           the results of the simulation.) */
		++num_in_system;

        delay            = 0.0;
        total_of_delays += delay;

		if(delay>excess_delay)
		{
			num_delayed_excess++;
		}

        /* Increment the number of customers delayed, and make server busy. */

        ++num_custs_delayed;
        server_status = BUSY;

        /* Schedule a departure (service completion). */

        time_next_event[2] = sim_time + expon(mean_service,seed);

		/*time_in_system=time_next_event[2]-sim_time;
		total_time_in_system += time_in_system;

		if(time_in_system>max_time_in_system)
		{
			max_time_in_system=time_in_system;
		}*/

		time_arrival[num_in_q] = sim_time;
    }
}

void QueueingClass::getResult(ResultHandler* result)
{

	result->avg_delay = total_of_delays / num_custs_delayed;
	
	result->avg_number=area_num_in_q / sim_time;
	
	result->avg_time_num_in_sys=area_num_in_system / sim_time;
	
	result->avg_total_time_in_sys=total_time_in_system / num_custs_served;
	
	result->max_queue_length=(float)max_num_in_q;
	
	result->max_delay_in_queue=max_delay_in_q;
	
	result->max_time_int_sys=max_time_in_system;

	result->prop_cus_delay_excess=(float)num_delayed_excess / num_custs_delayed;

	result->server_util=area_server_status / sim_time;

	result->sim_tim=sim_time;

	result->balk=(float)num_of_balk;
	
}

ResultHandler FixedCustomerQueueing::bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter){
	ResultHandler result;

	num_events = 2;
	this->mean_interarrival = mean_interarrival_in;
	this->mean_service = mean_service_in;
	this->num_delays_required = third_parameter;
	
    initialize(seed);

    /* Run the simulation while more delays are still needed. */

    while (num_custs_delayed < num_delays_required)     
    {
        /* Determine the next event. */

        timing();

        /* Update time-average statistical accumulators. */

        update_time_avg_stats();

        /* Invoke the appropriate event function. */

        switch (next_event_type) 
        {
            case 1:
                arrive(seed);
                break;
            case 2:
                depart(seed);
                break;
        }
    }
	result.seed = seed;
	getResult(&result);
	return result;
}

ResultHandler FixedLengthQueueing::bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter){
	ResultHandler result;
	num_events = 2;
	
	result.seed = seed;
	this->mean_interarrival = mean_interarrival_in;
	this->mean_service = mean_service_in;
    initialize(seed);

    /* Run the simulation while more delays are still needed. */

	while (sim_time < third_parameter)     
    {
        /* Determine the next event. */

        timing();

        /* Update time-average statistical accumulators. */

        update_time_avg_stats();

        /* Invoke the appropriate event function. */

        switch (next_event_type) 
        {
            case 1:
                arrive(seed);
                break;
            case 2:
                depart(seed);
                break;
        }
    }

    /* Invoke the report generator and end the simulation. */
	getResult(&result);
	return result;
}

ResultHandler LimitOpenTimeQueueing::bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter){
	ResultHandler result;
    initialize(seed);
	result.seed = seed;
	this->mean_interarrival = mean_interarrival_in;
	this->mean_service = mean_service_in;
    num_events = 2;

    initialize(seed);

    /* Run the simulation while more delays are still needed. */

    while (sim_time < (m_close_time-m_open_time)*60)     
    {
        /* Determine the next event. */

        timing();

        /* Update time-average statistical accumulators. */

        update_time_avg_stats();

        /* Invoke the appropriate event function. */

        switch (next_event_type) 
        {
            case 1:
                arrive(seed);
                break;
            case 2:
                depart(seed);
                break;
        }
    }

	while(server_status   == BUSY)
	{
		timing2();

		update_time_avg_stats();

		depart(seed);

	}
	getResult(&result);
	return result;
}

ResultHandler LimitCapacityQueueing::bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter){
	ResultHandler result;
	
	result.seed = seed;
	this->mean_interarrival = mean_interarrival_in;
	this->mean_service = mean_service_in;
    num_events = 2;

    initialize(seed);

    /* Run the simulation while more delays are still needed. */

    while (sim_time < sim_time_required)     
    {
        /* Determine the next event. */

        timing3();

        /* Update time-average statistical accumulators. */

        update_time_avg_stats();

        /* Invoke the appropriate event function. */

        switch (next_event_type) 
        {
            case 1:
                arrive2(seed);
                break;
            case 2:
                depart(seed);
                break;
			case 3:
				break;
        }
    }
	getResult(&result);
	return result;
}