package solution1;

public class Table {
	private int nbEating = 0;
	private static Table INSTANCE = new Table();
	
	private Table() {
		this.nbEating = 0;
	}
	
	public static Table getInstance() {
		return INSTANCE;
	}
	
	public synchronized void requestAccess() {
		if(this.nbEating > Philosophe.NB_PHILOSOPHES/2) {
			try {
				this.wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		this.nbEating++;
	}
	
	public synchronized void leave() {
		this.nbEating--;
		this.notify();
	}
	
}
