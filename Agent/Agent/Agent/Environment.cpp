// Environment.cpp

#include "Environment.h"

template<class T>
Environment<T>::Environment() 
{

}

template<class T>
void Environment<T>::SetCurrState(State& newState) 
{
	currState = newState;
}

template<class T>
void Environment<T>::SetReturnedReward(T newReward)
{
	reward.SetReward(newReward);
}

template<class T>
void Environment<T>::AddAction()
{
	long newAction = actions.size();
	actions.push_back(newAction);
}

template<class T>
void Environment<T>::AddActions(int numOfActions)
{
	for (int i = 0; i < numOfActions; i++)
	{
		long newAction = actions.size();
		actions.push_back(newAction);
	}
}

template<class T>
void Environment<T>::RemoveAction()
{
	actions.pop_back();
}


template<class T>
void Environment<T>::RemoveActions(int numOfActions)
{
	for (int i = 0; i < numOfActions; i++)
	{
		if (actions.empty())
		{
			// TODO: Error
			break;
		}

		actions.pop_back();
	}
}

template class Environment<int>;
