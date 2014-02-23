package tests;

import java.util.ArrayList;
import java.util.List;

import main.Semaphore;
import junit.framework.TestCase;

public class TestSemaphore extends TestCase {
	private static final int NB_THREADS = 10;

	public static void test() throws InterruptedException {
		Semaphore sema = new Semaphore(1);
		List<TestThread> threads = new ArrayList<TestThread>();
		for(int i=0; i<NB_THREADS; i++) {
			threads.add(new TestThread(sema, "Thread"+i));
		}
		for(TestThread thread: threads) {
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			System.out.println(thread.getMessage()+" started.");
			thread.start();
		}
		for(TestThread thread: threads) {
			thread.join();
		}
	}
}
