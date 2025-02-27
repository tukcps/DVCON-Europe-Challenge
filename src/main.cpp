/*
 * main.cpp
 *
 *  Created on: 10.07.2024
 *      Author: herzog
 */


#include <systemc>

SC_MODULE(TestbenchModule) {
    sc_core::sc_port<sc_core::sc_signal_in_if<int>> status_input;
    double powerEstimation = 0.0;
    double energyEstimation = 0.0;

    SC_CTOR(TestbenchModule) : status_input("input") {
        SC_THREAD(processing);
        sensitive << status_input; // triggered by value change on the channel
        dont_initialize();
    }

    void processing() {
        while (true) {
            int status = status_input->read();
            //TODO to be filled by you!

            switch(status) {
                case 0: //Boot Status

                    break;
                case 1: //Not Available

                    break;
                case 2: //Available

                    break;
                case 3: //At University

                    break;
                case 4: //Bluetooth

                    break;
            }

            wait();
        }
    }
};

SC_MODULE(QUEUE) {
    sc_core::sc_event_queue eq;
    int status = -1;
    sc_core::sc_port<sc_core::sc_signal_out_if<int>> status_out;

    SC_CTOR(QUEUE) : status_out("out"){
        SC_THREAD(trigger);
        SC_THREAD(catch_eq);
        sensitive << eq; // cach_eq() will be triggered by event queue eq
        dont_initialize(); // don't run catch_eq() during initialization phase
    }

    void trigger(){
        while (true) {
            eq.notify(1, sc_core::SC_SEC);
            eq.notify(2, sc_core::SC_SEC);
            eq.notify(3600, sc_core::SC_SEC);
            eq.notify(3602, sc_core::SC_SEC);
            wait(3602, sc_core::SC_SEC); // another round
        }
    }

    void catch_eq(){
        while (true) {
            status_out->write(status);

            if(status==4)
                status=1;
            else
                status++;

            wait(); // wait for eq
        }
    }
};


int sc_main(int argc, char* argv[]) {
	sc_core::sc_set_time_resolution(1.0, sc_core::SC_SEC);

    sc_core::sc_signal<int> signal;

	QUEUE queue("queue"); // instantiate object
	TestbenchModule testbench("testbench");
    queue.status_out(signal);
    testbench.status_input(signal);


	std::cout << "Simulation started..." << std::endl;
    sc_core::sc_start(129600000, sc_core::SC_SEC);
	std::cout << "Simulation finished." << std::endl;
    std::cout << "-----------------------------" << std::endl << std::endl;

    std::cout << "Calculated Power: "<<testbench.powerEstimation<<" W"<<std::endl;
    std::cout << "Calculated Energy: "<<testbench.energyEstimation<<" J"<<std::endl;
	return 0;
}


