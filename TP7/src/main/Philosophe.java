package main;

public class Philosophe extends Thread {
	private static final int NB_PHILOSOPHES = 10;
	private static final int EAT_DURATION = 500;
	private static final int THINK_DURATION = 1000;
	private static final int NB_CYCLES = 3;
	private static final int TAKE_BAGUETTE_DURATION = 0;
	private static boolean[] freeBaguettes = new boolean[NB_PHILOSOPHES];
	private int num;
	
	public Philosophe(int i) {
		this.num = i;
	}
	
	@Override
	public void run() {
		for(int i=0; i<NB_CYCLES; i++) {
			try {
				while(!takeBaguette(this.num % NB_PHILOSOPHES)) {
					this.wait();
				}
				while(!takeBaguette((this.num+1) % NB_PHILOSOPHES)) {
					this.wait();
				}
			} catch (InterruptedException e1) {
				e1.printStackTrace();
			}
			System.out.println("Philosophe "+this.num+" is eating...");
			try {
				Thread.sleep(EAT_DURATION);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			
			this.freeBaguettes();
			System.out.println("Philosophe "+this.num+" is now thinking...");
			try {
				Thread.sleep(THINK_DURATION);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	private static boolean takeBaguette(int baguette) throws InterruptedException {
		if(freeBaguettes[baguette]) {
			Thread.sleep(TAKE_BAGUETTE_DURATION);
			freeBaguettes[baguette] = false;
			return true;
		}
		return false;
	}
	
	private void freeBaguettes() {
		freeBaguettes[this.num % NB_PHILOSOPHES] = true;
		freeBaguettes[(this.num+1) % NB_PHILOSOPHES] = true;
		this.notifyAll();
	}
	
	public static void main(String[] args) throws InterruptedException {
		for(int i=0; i<NB_PHILOSOPHES; i++) {
			Philosophe.freeBaguettes[i] = true;
		}
		Philosophe[] philosophes = new Philosophe[NB_PHILOSOPHES];
		for(int i=0; i<NB_PHILOSOPHES; i++) {
			philosophes[i] = new Philosophe(i);
		}
		for(int i=0; i<NB_PHILOSOPHES; i++) {
			philosophes[i].start();
			Thread.sleep(100);
		}
		for(int i=0; i<NB_PHILOSOPHES; i++) {
			philosophes[i].join();
		}
	}
}
