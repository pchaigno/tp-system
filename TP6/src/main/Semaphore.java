package main;

import java.util.LinkedList;

public class Semaphore {
	private int i;
	private static LinkedList<Thread> threads = new LinkedList<Thread>();
	
	public Semaphore(int i) {
		this.i = i;
	}
	
	public synchronized void p() {
		if(this.i <= 0) {
			this.goToBed();
		}
		this.i--;
	}
	
	public synchronized void v() {
		this.i++;
		if(this.i > 0) {
			if(threads.size() > 0) {
				threads.poll().interrupt();
				this.goToBed();
			}
		}
	}
	
	public static void wakeAnotherOne() {
		if(threads.size() > 0) {
			threads.poll().interrupt();
		}
	}
	
	private void goToBed() {
		threads.add(Thread.currentThread());
		try {
			this.wait();
		} catch (InterruptedException e) {
			
		}
	}
}
