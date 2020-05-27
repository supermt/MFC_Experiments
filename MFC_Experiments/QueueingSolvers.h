#pragma once

#include "lcgrand.h"
#include <math.h>
#include <vector>

#define Q_LIMIT 100  /* Limit on queue length. */
#define BUSY      1  /* Mnemonics for server's being busy */
#define IDLE      0  /* and idle. */

struct ResultHandler{
public:
	float avg_delay;// total_of_delays / num_custs_delayed
	float time_avg_number_in_q; // area_num_in_q / sim_time for queue depth
	float server_util; // area_server_status / sim_time
	float time_avg_number_in_system; // area_num_in_q / sim_time, +1 for queue 
	float avg_time_in_system; // avg of overall_vec;
	float queue_length_max; // max of queue_length_vec
	float max_delay_in_queue; // max of delay_vec
	float max_time_in_system; // max of overall_vec
	float propo_customers_over_1min; // number of delay_vec[i] > 1 / len(delay_vec[i])
};

class QueueingClass{
public:
	QueueingClass();
	~QueueingClass();
	float expon(float mean);
	void update_time_avg_stats(void);
	void report(float* avg_delay,float* avg_num, float* server_util,float* sim_time_end);  /* Report generator function. */
	void fill_result(ResultHandler* result);
	void timing(void);
	void arrive(void);
	void depart(void);
	virtual void initialize()=0;
	virtual ResultHandler bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter)=0;
	
protected:
	FILE  *infile, *outfile;
	int   next_event_type, num_custs_delayed, num_events, num_in_q, server_status;
	float sim_time, time_last_event, area_num_in_q, area_server_status;
	float mean_interarrival, mean_service, time_arrival[Q_LIMIT + 1],total_of_delays;
	float *time_next_event;
	std::vector<float> delay_vec;
	std::vector<float> queue_length_vec;
	std::vector<float> serve_vec;
	std::vector<float> overall_vec;
	
};

class FixedCustomerQueueing :public QueueingClass
{
public:
	FixedCustomerQueueing();
	~FixedCustomerQueueing();
	void initialize();
	ResultHandler bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter);
private:
	int   num_delays_required;
};

class FixedLengthQueueing :public QueueingClass
{
public:
	FixedLengthQueueing();
	~FixedLengthQueueing();
	void initialize();
	ResultHandler bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter);
private:
	float time_end, time_last_event;
};
