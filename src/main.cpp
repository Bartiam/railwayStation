#include <iostream>
#include <vector>
#include <thread>
#include <ctime>
#include <mutex>

std::mutex mtx;

void railway_station(const int& time, const char& name, std::time_t currentTime)
{
	std::time_t arrivalTime = currentTime + time;
	std::string command;

	while (true)
	{
		currentTime = std::time(NULL);
		if (currentTime == arrivalTime)
		{
			std::cout << "\nThe train \"" << name << "\" is waiting free way at the train station. ";
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			mtx.lock();

			while (true)
			{
				std::cout << "\nThe train \"" << name << "\" has arrived at the station. ";

				std::cin >> command;
				if (command != "Depart" && command != "depart")
					std::cerr << "Error! Incorrect input command. Try again. " << std::endl;
				else
				{
					std::cout << "The train \"" << name << "\" left the station. ";
					mtx.unlock();
					return;
				}
			}
		}
	}
}

int main()
{
	std::vector<std::pair<char, int>> trains(3);
	trains[0].first = 'A';
	trains[1].first = 'B';
	trains[2].first = 'C';

	for (int i = 0; i < trains.size(); i++)
	{
		std::cout << "Enter the time before arrival at the train \"" << trains[i].first << "\" station: ";
		std::cin >> trains[i].second;
		if (trains[i].second <= 0)
		{
			std::cerr << "Error! Incorrect input data. Try again. " << std::endl;
			--i;
		}
	}

	std::time_t currentTime = std::time(NULL);

	std::thread firstTrain(railway_station, trains[0].second, trains[0].first, currentTime);
	std::thread secondTrain(railway_station, trains[1].second, trains[1].first, currentTime);
	railway_station(trains[2].second, trains[2].first, currentTime);

	firstTrain.join();
	secondTrain.join();

	return 0;
}