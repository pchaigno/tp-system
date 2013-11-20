package solution1;

public class Baguette {
	private static final int TAKE_BAGUETTE_DURATION = 100;
	private boolean free;
	
	public Baguette() {
		this.free = true;
	}
	
	public synchronized void take() {
		if(!this.free) {
			try {
				this.wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		try {
			Thread.sleep(TAKE_BAGUETTE_DURATION);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		this.free = false;
	}
	
	public synchronized void free() {
		this.free = true;
		this.notify();
	}
}
