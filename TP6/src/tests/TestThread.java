package tests;

import main.Semaphore;

public class TestThread extends Thread {
	private String message;
	private Semaphore sema;
	private static final int NB_MESSAGES = 5;
	private static final int SLEEP_TIME = 50;
	
	public TestThread(Semaphore sema, String message) {
		this.sema = sema;
		this.message = message;
	}
	
	@Override
	public void run() {
		this.sema.p();
		for(int i=0; i<NB_MESSAGES; i++) {
			System.out.println(this.message);
			try {
				Thread.sleep(SLEEP_TIME);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		this.sema.v();
		
		this.sema.p();
		for(int i=0; i<NB_MESSAGES; i++) {
			System.out.println(this.message);
			try {
				Thread.sleep(SLEEP_TIME);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		this.sema.v();
		Semaphore.wakeAnotherOne();
	}
	
	public String getMessage() {
		return this.message;
	}
}
