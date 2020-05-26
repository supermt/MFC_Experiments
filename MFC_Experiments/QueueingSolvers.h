#pragma once

#include "lcgrand.h"
#include <math.h>

#define Q_LIMIT 100  /* Limit on queue length. */
#define BUSY      1  /* Mnemonics for server's being busy */
#define IDLE      0  /* and idle. */

class QueueingClass{
public:
	virtual void initialize();
	QueueingClass(){	
		initialize();
	}
	~QueueingClass();
	float expon(float mean){return -mean * log(lcgrand(1));}

	void update_time_avg_stats(void);
	void report(float* avg_delay,float* avg_num, float* server_util,float* sim_time_end);  /* Report generator function. */

	void timing(void);
	void arrive(void);
	void depart(void);
	virtual void bootstrap();

protected:
	FILE  *infile, *outfile;
	int   next_event_type, num_custs_delayed, num_events, num_in_q, server_status;
	float sim_time, time_last_event, area_num_in_q, area_server_status;
	float mean_interarrival, mean_service, time_arrival[Q_LIMIT + 1],total_of_delays;
	float time_next_event[3];

};

class FixedCustomerQueueing :public QueueingClass
{
public:
	FixedCustomerQueueing();
	~FixedCustomerQueueing();
	void initialize();
	void bootstrap();
private:
	int   num_delays_required;
	float time_next_event[3],total_of_delays;
};

class FixedLengthQueueing :public QueueingClass
{
public:
	FixedLengthQueueing();
	~FixedLengthQueueing();
	void initialize();
	void bootstrap();
private:
	float time_end, time_last_event,time_next_event[4], total_of_delays;
};

void QueueingClass::update_time_avg_stats(){
	float time_since_last_event;
	/* Compute time since last event, and update last-event-time marker. */
	time_since_last_event = sim_time - time_last_event;
	time_last_event       = sim_time;
	/* Update area under number-in-queue function. */
	area_num_in_q      += num_in_q * time_since_last_event;
	/* Update area under server-busy indicator function. */
	area_server_status += server_status * time_since_last_event;
}

void QueueingClass::report(float* avg_delay,float* avg_num, float* server_util,float* sim_time_end){
	/* Compute and write estimates of desired measures of performance. */
	/*
	fprintf(outfile, "\n\nAverage delay in queue%11.3f minutes\n\n",
	total_of_delays / num_custs_delayed);
	fprintf(outfile, "Average number in queue%10.3f\n\n",
	area_num_in_q / sim_time);
	fprintf(outfile, "Server utilization%15.3f\n\n",
	area_server_status / sim_time);
	fprintf(outfile, "Time simulation ended%12.3f minutes", sim_time);
	*/
	*avg_delay = (total_of_delays / num_custs_delayed);
	*avg_num = (area_num_in_q / sim_time);
	*server_util = (area_server_status / sim_time);
	*sim_time_end = sim_time;
}

void QueueingClass::timing(){
	int   i;
	float min_time_next_event = 1.0e+29;

	next_event_type = 0;

	/* Determine the event type of the next event to occur. */

	for (i = 1; i <= num_events; ++i)
		if (time_next_event[i] < min_time_next_event) {
			min_time_next_event = time_next_event[i];
			next_event_type     = i;
		}

		/* Check to see whether the event list is empty. */

		if (next_event_type == 0)
		{ 

			/* The event list is empty, so stop the simulation */

			fprintf(outfile, "\nEvent list empty at time %f", sim_time);
			exit(1);
		}

		/* The event list is not empty, so advance the simulation clock. */

		sim_time = min_time_next_event;
}

void QueueingClass::arrive(){
	float delay;

	/* Schedule next arrival. */

	time_next_event[1] = sim_time + expon(mean_interarrival);

	/* Check to see whether server is busy. */

	if (server_status == BUSY)
	{
		/* Server is busy, so increment number of customers in queue. */

		++num_in_q;

		/* Check to see whether an overflow condition exists. */

		if (num_in_q > Q_LIMIT)
		{
			/* The queue has overflowed, so stop the simulation. */

			fprintf(outfile, "\nOverflow of the array time_arrival at");
			fprintf(outfile, " time %f", sim_time);
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

		/* Increment the number of customers delayed, and make server busy. */

		++num_custs_delayed;
		server_status = BUSY;

		/* Schedule a departure (service completion). */

		time_next_event[2] = sim_time + expon(mean_service);
	}
}

void QueueingClass::depart()
{
	int   i;
	float delay;

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

		/* Increment the number of customers delayed, and schedule departure. */

		++num_custs_delayed;
		time_next_event[2] = sim_time + expon(mean_service);

		/* Move each customer in queue (if any) up one place. */

		for (i = 1; i <= num_in_q; ++i)
			time_arrival[i] = time_arrival[i + 1];
	}
}
