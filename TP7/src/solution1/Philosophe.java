package solution1;

public class Philosophe extends Thread {
	public static final int NB_PHILOSOPHES = 10;
	private static final int EAT_DURATION = 500;
	private static final int THINK_DURATION = 1000;
	private static final int NB_CYCLES = 3;
	private static Baguette[] baguettes = new Baguette[NB_PHILOSOPHES];
	private int num;
	
	public Philosophe(int i) {
		this.num = i;
	}
	
	@Override
	public void run() {
		for(int i=0; i<NB_CYCLES; i++) {
			this.eat();
			this.think();
		}
	}
	
	private void eat() {
		Table.getInstance().requestAccess();
		baguettes[this.num].take();
		baguettes[(this.num+1) % NB_PHILOSOPHES].take();
		System.out.println("Philosophe "+this.num+" is eating...");
		try {
			Thread.sleep(EAT_DURATION);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		baguettes[this.num].free();
		baguettes[(this.num+1) % NB_PHILOSOPHES].free();
		Table.getInstance().leave();
	}
	
	private void think() {
		System.out.println("Philosophe "+this.num+" is now thinking...");
		try {
			Thread.sleep(THINK_DURATION);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) throws InterruptedException {
		Philosophe[] philosophes = new Philosophe[NB_PHILOSOPHES];
		for(int i=0; i<NB_PHILOSOPHES; i++) {
			Philosophe.baguettes[i] = new Baguette();
			philosophes[i] = new Philosophe(i);
		}
		for(int i=0; i<NB_PHILOSOPHES; i++) {
			philosophes[i].start();
			Thread.sleep(50);
		}
		for(int i=0; i<NB_PHILOSOPHES; i++) {
			philosophes[i].join();
		}
	}
}
