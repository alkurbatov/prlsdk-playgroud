#include <unistd.h>
#include "test.h"
#include "collection.h"

namespace
{

struct Input
{
	Server* srv;
	std::string target;
	std::string mark;
};

void* do_exec(void* data)
{
	Input* d = static_cast<Input*>(data);

	PRL_HANDLE v;
	if (PRL_FAILED(d->srv->find(d->target, v)))
		return NULL;

	Vm a(v, SYNC);
	a.set_mark(d->mark);

	Session::Unit* m = NULL;

	a.login("root", "1q2w3e", &m);
	a.set_mode(SYNC);

	m->set_mark(d->mark);

	PRL_RESULT r = a.connect_display();

//	sleep(5);

	m->run("hostname");

	sleep(2);
	a.disconnect_display();
	m->logout();

	delete m;
}

} // namespace

namespace Test
{
namespace Display
{

void connect_on_stopped(PRL_HANDLE ve_)
{
	std::cout << "Test: connect_on_stopped_vm: SYNC"<< std::endl;

	Vm a(ve_, SYNC);
	a.stop();

	a.connect_display();
	a.start();
	a.connect_display();

	std::cout << "Test: connect_on_stopped_vm: ASYNC"<< std::endl;
	a.stop();

	a.set_mode(ASYNC);
	a.connect_display();
	sleep(2);

	a.set_mode(SYNC);
	a.start();
	sleep(2);

	a.set_mode(ASYNC);
	a.connect_display();
	sleep(2);
}

} // namespace Display

namespace Exec
{

void basic(Server* srv_, const std::string& target_)
{
	PRL_HANDLE v;
	if (PRL_FAILED(srv_->find(target_, v)))
		return;

	Vm a(v, SYNC);

	Session::Unit* m = NULL;

	a.login("root", "1q2w3e", &m);

	PRL_RESULT r = a.connect_display();
	std::cout << "DISPLAY_CONNECT RetCode " << r << std::endl; 

	m->run("hostname");

	a.disconnect_display();
	m->logout();

	delete m;
}

void multythread(Server* srv_, const std::string& target_)
{
	pthread_t t1, t2, t3, t4, t5, t6;

	Input d1;
	d1.srv = srv_;
	d1.target = target_;
	d1.mark = "Thread 1";

	if (pthread_create(&t1, NULL, do_exec, &d1)) {
		std::cerr << "Failed to create t1" << std::endl;
	}

//	usleep(200);

	Input d2;
	d2.srv = srv_;
	d2.target = target_;
	d2.mark = "Thread 2";

	if (pthread_create(&t2, NULL, do_exec, &d2)) {
		std::cerr << "Failed to create t2" << std::endl;
	}

	usleep(700);

	Input d3;
	d3.srv = srv_;
	d3.target = target_;
	d3.mark = "Thread 3";

	if (pthread_create(&t3, NULL, do_exec, &d3)) {
		std::cerr << "Failed to create t3" << std::endl;
	}

	Input d4;
	d4.srv = srv_;
	d4.target = target_;
	d4.mark = "Thread 4";

	if (pthread_create(&t4, NULL, do_exec, &d4)) {
		std::cerr << "Failed to create t4" << std::endl;
	}

	sleep(1);

	Input d5;
	d5.srv = srv_;
	d5.target = target_;
	d5.mark = "Thread 5";

	if (pthread_create(&t5, NULL, do_exec, &d5)) {
		std::cerr << "Failed to create t5" << std::endl;
	}

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);
	pthread_join(t5, NULL);

	sleep(3);

	Input d6;
	d6.srv = srv_;
	d6.target = target_;
	d6.mark = "Thread 6";

	if (pthread_create(&t6, NULL, do_exec, &d6)) {
		std::cerr << "Failed to create t5" << std::endl;
	}

	pthread_join(t6, NULL);
}

} // namespace Exec

} // namespace Test

