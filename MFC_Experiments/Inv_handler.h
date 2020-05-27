#pragma once
class Inv_handler
{
public:
	Inv_handler();
	void set_parameters(int initial_inv_level, int num_months, int num_policies, int num_values_demand,
		float mean_interdemand, float setup_cost, float incremental_cost, float holding_cost,
		float shortage_cost, float minlag, float maxlag);
	~Inv_handler(void);
	void bootstrap();
	void fill_prob_distri_function(float input_prob_func[]);

private:
	int   amount, bigs, initial_inv_level, inv_level, next_event_type, num_events,
		num_months, num_values_demand, smalls;
	float area_holding, area_shortage, holding_cost, incremental_cost, maxlag,
		mean_interdemand, minlag, prob_distrib_demand[26], setup_cost,
		shortage_cost, sim_time, time_last_event, time_next_event[5],
		total_ordering_cost;
	FILE  *infile, *outfile;
	int num_policies;

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

