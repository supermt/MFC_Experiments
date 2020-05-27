#pragma once
class Inv_handler
{
public:
	Inv_handler(void);
	~Inv_handler(void);
	void bootstrap();

private:
	int   amount, bigs, initial_inv_level, inv_level, next_event_type, num_events,
		num_months, num_values_demand, smalls;
	float area_holding, area_shortage, holding_cost, incremental_cost, maxlag,
		mean_interdemand, minlag, prob_distrib_demand[26], setup_cost,
		shortage_cost, sim_time, time_last_event, time_next_event[5],
		total_ordering_cost;
	FILE  *infile, *outfile;

	void  initialize(void);
	void  timing(void);
	void  order_arrival(void);
	void  demand(void);
	void  evaluate(void);
	void  report(void);
	void  update_time_avg_stats(void);
	float expon(float mean);
	int   random_integer(float prob_distrib []);
	float uniform(float a, float b);
};

