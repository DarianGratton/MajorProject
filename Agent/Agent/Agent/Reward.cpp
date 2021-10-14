// Reward.cpp

#include "Reward.h"

template<class T>
Reward<T>::Reward(T reward) : reward(reward)
{
	
}

template class Reward<int>;
template class Reward<float>;
template class Reward<long>;
template class Reward<double>;
