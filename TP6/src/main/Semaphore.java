package main;

import java.util.LinkedList;

import tests.TestThread;

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
		System.out.println(((TestThread)Thread.currentThread()).getMessage()+" going down ("+this.i+").");
		threads.add(Thread.currentThread());
		try {
			this.wait();
		} catch (InterruptedException e) {
			System.out.println(((TestThread)Thread.currentThread()).getMessage()+" waking up ("+this.i+").");
		}
	}
}
