#include "stdafx.h"
#include "QueueingSolvers.h"
#include <numeric>
#include <algorithm>
#define SEED_ARRAY_LENGTH 101
#include "RandomStream.h"



QueueingClass::QueueingClass(){
	delay_vec = std::vector<float>();
	queue_length_vec = std::vector<float>();
	serve_vec = std::vector<float>();
	overall_vec = std::vector<float>();
}
FixedCustomerQueueing::FixedCustomerQueueing(){
	fopen_s(&outfile,"mm1.out", "w");
	time_next_event = new float[3];
}
FixedLengthQueueing::FixedLengthQueueing(){
	fopen_s(&outfile,"mm1alt.out", "w");
	time_next_event = new float[4];
}
QueueingClass::~QueueingClass(){
	delete(time_next_event);
}
FixedCustomerQueueing::~FixedCustomerQueueing(){
}
FixedLengthQueueing::~FixedLengthQueueing(){
}

float QueueingClass::expon(float mean){
	return -mean * log(RandomGenerator::lcgrand(1));
}


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

	fprintf(outfile, "\n\nAverage delay in queue%11.3f minutes\n\n",
		total_of_delays / num_custs_delayed);
	fprintf(outfile, "Average number in queue%10.3f\n\n",
		area_num_in_q / sim_time);
	fprintf(outfile, "Server utilization%15.3f\n\n",
		area_server_status / sim_time);
	fprintf(outfile, "Time simulation ended%12.3f minutes", sim_time);
	*avg_delay = (total_of_delays / num_custs_delayed);
	*avg_num = (area_num_in_q / sim_time);
	*server_util = (area_server_status / sim_time);
	*sim_time_end = sim_time;
}

inline float avg(std::vector<float> *v)
{
	int n = v->size();	
	if (n==0){
		return 0;
	}

	return	accumulate(v->begin(), v->end(), 0.0) / n;

}

inline float max_from_vec(std::vector<float>* v)
{
	float max_float = *std::max_element(v->begin(),v->end());
	return max_float;
}

void QueueingClass::fill_result(ResultHandler* result){
	result->avg_delay = total_of_delays / num_custs_delayed;
	result->time_avg_number_in_q = area_num_in_q/sim_time;
	result->server_util = area_server_status / sim_time;
	result->time_avg_number_in_system = area_num_in_q / sim_time + 1;
	result->avg_time_in_system =  avg(&overall_vec);
	result->queue_length_max = max_from_vec(&queue_length_vec);
	result->max_delay_in_queue = max_from_vec(&delay_vec);
	result->max_time_in_system = max_from_vec(&overall_vec);
	int customer_over_1min = 0;
	for (float overall_time : delay_vec){
		if (overall_time>1) customer_over_1min++;
	}
	if (delay_vec.size()!=0) result->propo_customers_over_1min = float(customer_over_1min)/delay_vec.size();
	else result->propo_customers_over_1min = 0;
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
			//return ;
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

		this->queue_length_vec.push_back(1+num_in_q);
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
		float serve_time = expon(mean_service);
		this->serve_vec.push_back(serve_time);
		if (delay_vec.size()>=1) this->overall_vec.push_back(serve_time+delay_vec.back());
		else this->overall_vec.push_back(serve_time);
		time_next_event[2] = sim_time + serve_time;
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
		this->delay_vec.push_back(delay);
		total_of_delays += delay;

		/* Increment the number of customers delayed, and schedule departure. */

		++num_custs_delayed;
		time_next_event[2] = sim_time + expon(mean_service);

		/* Move each customer in queue (if any) up one place. */

		for (i = 1; i <= num_in_q; ++i)
			time_arrival[i] = time_arrival[i + 1];
	}
}

void FixedCustomerQueueing::initialize(){
	/* Initialize the simulation clock. */

	sim_time = 0.0;

	/* Initialize the state variables. */

	server_status   = IDLE;
	num_in_q        = 0;
	time_last_event = 0.0;

	/* Initialize the statistical counters. */

	num_custs_delayed  = 0;
	total_of_delays    = 0.0;
	area_num_in_q      = 0.0;
	area_server_status = 0.0;

	/* Initialize event list.  Since no customers are present, the departure
	(service completion) event is eliminated from consideration. */

	time_next_event[1] = sim_time + expon(mean_interarrival);
	time_next_event[2] = 1.0e+30;
}

void FixedLengthQueueing::initialize(){
	/* Initialize the simulation clock. */

	sim_time = 0.0;

	/* Initialize the state variables. */

	server_status   = IDLE;
	num_in_q        = 0;
	time_last_event = 0.0;

	/* Initialize the statistical counters. */

	num_custs_delayed  = 0;
	total_of_delays    = 0.0;
	area_num_in_q      = 0.0;
	area_server_status = 0.0;

	/* Initialize event list.  Since no customers are present, the departure
	(service completion) event is eliminated from consideration.  The end-
	simulation event (type 3) is scheduled for time time_end. */

	time_next_event[1] = sim_time + expon(mean_interarrival);
	time_next_event[2] = 1.0e+30;
	time_next_event[3] = time_end;

}

ResultHandler FixedCustomerQueueing::bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter){
	/* Open input and output files. */
	this->mean_interarrival = mean_interarrival_in;
	this->mean_service = mean_service_in;
	this->num_delays_required = int(third_parameter);
	/* Specify the number of events for the timing function. */

	num_events = 2;

	/* Write report heading and input parameters. */

	fprintf(outfile, "Single-server queueing system\n\n");
	fprintf(outfile, "Mean interarrival time%11.3f minutes\n\n",
		mean_interarrival);
	fprintf(outfile, "Mean service time%16.3f minutes\n\n", mean_service);
	fprintf(outfile, "Number of customers%14d\n\n", num_delays_required);

	/* Initialize the simulation. */

	initialize();

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
			arrive();
			break;
		case 2:
			depart();
			break;
		}
	}

	/* Invoke the report generator and end the simulation. */
	float avg_delay, avg_num,  server_util, sim_time_end;
	report(&avg_delay,&avg_num,&server_util,&sim_time_end);

	fflush(outfile);
	fclose(outfile);
	ResultHandler result = ResultHandler();
	fill_result(&result);

	return result;
}

ResultHandler FixedLengthQueueing::bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter){
	/* Open input and output files. */

	/* Specify the number of events for the timing function. */

	num_events = 3;

	float avg_delay, avg_num,  server_util, sim_time_end;
	/* Read input parameters. */
	this->mean_interarrival = mean_interarrival_in;
	this->mean_service = mean_service_in;
	this->time_end = third_parameter;

	/* Write report heading and input parameters. */

	fprintf(outfile, "Single-server queueing system with fixed run");
	fprintf(outfile, " length\n\n");
	fprintf(outfile, "Mean interarrival time%11.3f minutes\n\n",
		mean_interarrival);
	fprintf(outfile, "Mean service time%16.3f minutes\n\n", mean_service);
	fprintf(outfile, "Length of the simulation%9.3f minutes\n\n", time_end);

	/* Initialize the simulation. */

	initialize();

	/* Run the simulation until it terminates after an end-simulation event
	(type 3) occurs. */

	ResultHandler result = ResultHandler();
	do
	{
		/* Determine the next event. */

		timing();

		/* Update time-average statistical accumulators. */

		update_time_avg_stats();

		/* Invoke the appropriate event function. */

		switch (next_event_type)
		{
		case 1:
			arrive();
			break;
		case 2:
			depart();
			break;
		case 3:
			report(&avg_delay,&avg_num,&server_util,&sim_time_end);
			fill_result(&result);
			break;
		}

		/* If the event just executed was not the end-simulation event (type 3),
		continue simulating.  Otherwise, end the simulation. */

	} while (next_event_type != 3);

	fflush(outfile);
	fclose(outfile);


	return result;
}