#include "TestEnvironment.h"

TestEnvironment::TestEnvironment(float x, float y)
{
    goal << x, y;
	pos.setZero();
    state << pos, goal;

	old_dist = GoalDist(pos);
}

std::tuple<torch::Tensor, int, torch::Tensor> TestEnvironment::Act(float act_x, float act_y)
{
    old_dist = GoalDist(pos);

    // float max_step = 0.1f; If Continous
    float max_step = 1; // If Discrete
    pos(0) += max_step * act_x;
    pos(1) += max_step * act_y;

    state << pos, goal;

    torch::Tensor newState = State();
    torch::Tensor done = torch::zeros({ 1, 1 }, torch::kF32);
    STATUS status;

    if (GoalDist(pos) < 6e-1) 
    {
        status = WON;
        done[0][0] = 1.;
    }
    else if (GoalDist(pos) > 1e1) 
    {
        status = LOST;
        done[0][0] = 1.;
    }
    else 
    {
        status = PLAYING;
        done[0][0] = 0.;
    }

    return std::make_tuple(newState, status, done);
}

torch::Tensor TestEnvironment::State()
{
    torch::Tensor newState = torch::zeros({ 1, state.size() }, torch::kF32);
    std::memcpy(newState.data_ptr(), state.data(), state.size() * sizeof(float));
    return newState;
}

torch::Tensor TestEnvironment::Reward(int status)
{
    torch::Tensor reward = torch::full({ 1, 1 }, old_dist - GoalDist(pos), torch::kF32);

    switch (status)
    {
    case PLAYING:
        break;
    case WON:
        reward[0][0] += 10.;
        printf("won, reward: %f\n", reward[0][0].item<float>());
        break;
    case LOST:
        reward[0][0] -= 10.;
        printf("lost, reward: %f\n", reward[0][0].item<float>());
        break;
    }

    return reward;
}

float TestEnvironment::GoalDist(Eigen::Vector2f& x)
{
    return (goal - x).norm();
}

void TestEnvironment::Reset()
{
    pos.setZero();
    state << pos, goal;
}

void TestEnvironment::SetGoal(float x, float y)
{
    goal(0) = x;
    goal(1) = y;

    old_dist = GoalDist(pos);
    state << pos, goal;
}
