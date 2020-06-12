#pragma once

#include <math.h>
#include <vector>
#define MODLUS 2147483647
#define MULT1       24112
#define MULT2       26143
#define Q_LIMIT 1000  /* Limit on queue length. */
#define BUSY      1  /* Mnemonics for server's being busy */
#define IDLE      0  /* and idle. */


extern long default_seeds[101];

struct ResultHandler{
public:
	float seed;
	float avg_delay;// total_of_delays / num_custs_delayed
	float avg_number; // area_num_in_q / sim_time for queue depth
	float avg_time_num_in_sys;
	float avg_total_time_in_sys;
	float max_queue_length;
	float max_delay_in_queue;
	float max_time_int_sys;
	float prop_cus_delay_excess;
	float server_util;
	float sim_tim;
	float balk;
};

class QueueingClass{
public:
	QueueingClass();
	~QueueingClass();

	int   next_event_type, num_custs_delayed, num_delayed_excess, num_delays_required, num_events,
	num_in_q, max_num_in_q, num_in_system, server_status, num_custs_served,sim_time_required,
	num_of_balk, queue_capacity;
	float area_num_in_q, area_num_in_system, area_server_status, mean_interarrival, mean_service,
	sim_time, time_arrival[Q_LIMIT + 1], time_last_event, time_next_event[3],
	total_of_delays, time_in_system, total_time_in_system, excess_delay, max_delay_in_q,
	max_time_in_system;
	float result[Q_LIMIT + 1][12];
	int m_open_time;
	int m_close_time;
	int seed;
	

	void  initialize(int seed);
	void  timing(void);
	void  timing2(void);
	void  arrive(int seed);
	void  arrive2(int seed);
	void  depart(int seed);
	void  update_time_avg_stats(void);
	float expon(float mean,int seed);
	void timing3(void)  ;
	void getResult(ResultHandler* result);
	virtual ResultHandler bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter)=0;
};


class FixedCustomerQueueing :public QueueingClass
{
public:
	FixedCustomerQueueing();
	~FixedCustomerQueueing();
	ResultHandler bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter);
private:
	int   num_delays_required;
};

class FixedLengthQueueing :public QueueingClass
{
public:
	FixedLengthQueueing();
	~FixedLengthQueueing();
	ResultHandler bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter);
};

class LimitOpenTimeQueueing: public QueueingClass
{
public:
	LimitOpenTimeQueueing();
	~LimitOpenTimeQueueing();
	ResultHandler bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter);

};

class LimitCapacityQueueing: public QueueingClass
{
public: 
	LimitCapacityQueueing();
	~LimitCapacityQueueing();
	ResultHandler bootstrap(float mean_interarrival_in, float mean_service_in,float third_parameter);
};

