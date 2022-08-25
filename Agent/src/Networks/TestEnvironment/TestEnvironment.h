#pragma once

#include <Eigen/Core>
#include <torch/torch.h>

/*
  TestEnvironment

  Third-party environment takening from github user mhubii.

  TODO: Replace with a system that is easier to use and allows for 
  testing with discrete and continous action spaces. (Major Project: Out-of-Scope)

  Source: https://github.com/mhubii/ppo_libtorch
*/
class TestEnvironment
{
public:
	TestEnvironment(float x, float y);

	std::tuple<torch::Tensor, int, torch::Tensor> Act(float act_x, float act_y);
	
	torch::Tensor State();
	torch::Tensor Reward(int status);
	
	float GoalDist(Eigen::Vector2f& x);
	void Reset();
	void SetGoal(float x, float y);

	enum STATUS
	{
		PLAYING,
		WON,
		LOST,
		RESETTING
	};

	Eigen::Vector2f pos;
	Eigen::Vector2f goal;
	Eigen::VectorXf state{ 2, 2 };
	float old_dist;
};