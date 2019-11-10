#include "Application.h"

enum class MainStatus {
	CREATION,
	START,
	UPDATE,
	FINISH,
	EXIT
};

Application* App = nullptr;

int main(int argc, char** argv) {

	int mainReturn = EXIT_FAILURE;
	MainStatus state = MainStatus::CREATION;

	while (state != MainStatus::EXIT) {
		switch (state) {
		case MainStatus::CREATION:
			LOG("-------------- Application Creation --------------\n");
			App = new Application();
			state = MainStatus::START;
			break;

		case MainStatus::START:
			LOG("-------------- Application Init --------------\n");
			if (App->Init() == false) {
				LOG("----- Application Init exits with error -----\n");
				state = MainStatus::EXIT;
			}
			else {
				state = MainStatus::UPDATE;
				LOG("-------------- Application Update --------------\n");
			}
			break;

		case MainStatus::UPDATE: {
			int updateReturn = (int)App->Update();
			if (updateReturn == (int)UpdateStatus::FAIL) {
				LOG("----- Application Update exits with error -----\n");
				state = MainStatus::EXIT;
			}
			if (updateReturn == (int)UpdateStatus::STOP)
				state = MainStatus::FINISH;
		}
		break;

		case MainStatus::FINISH:
			LOG("-------------- Application CleanUp --------------\n");
			if (!App->CleanUp()) {
				LOG("----- Application CleanUp exits with error -----\n");
			}
			else mainReturn = EXIT_SUCCESS;
			state = MainStatus::EXIT;
			break;
		}
	}

	delete App;
	//LOG("Bye :)\n");
	return mainReturn;
}
