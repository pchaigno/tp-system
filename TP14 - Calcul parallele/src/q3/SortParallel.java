package q3;

import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

public class SortParallel {
	private int[] table;
	private Vector<Task> tasks;
	private int nbToSort;
	
	public class Task {
		int deb;
		int end;
		
		public Task(int deb, int end) {
			this.deb = deb;
			this.end = end;
		}
	}
	
	public class SortThread extends Thread {
		@Override
		public void run() {
			while(nbToSort > 0) {
				Task nextTask;
				if((nextTask = getNextTask()) == null) {
					System.out.println("wait");
					waitBidon();
				} else {
					int deb = nextTask.deb;
					int end = nextTask.end;
					quickSort(deb, end);
				}
			}
		}
		
	}
	
	public SortParallel(int[] table) {
		this.table = table;
		this.tasks = new Vector<Task>();
	}
	
	public synchronized Task getNextTask() {
		if(this.tasks.size() == 0) {
			return null;
		}
		
		return this.tasks.remove(0);
	}
	
	public synchronized void oneMoreSorted() {
		this.nbToSort--;
	}
	
	public synchronized void waitBidon() {
		try {
			this.wait();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	public synchronized void notifyBidon() {
		this.notifyAll();
	}
	
	public int[] quickSort(int nbThreads) throws InterruptedException {
		int longueur = table.length;
		//quickSort(0, longueur-1);
		this.tasks.add(new Task(0, longueur-1));
		this.nbToSort = this.table.length + 2;
		
		List<SortThread> threads = new ArrayList<SortThread>();
		for(int i=0; i<nbThreads; i++) {
			SortThread thread = new SortThread();
			threads.add(thread);
			thread.start();
		}
		
		for(SortThread thread: threads) {
			thread.join();
		}
		return table;
	}

	private int partition(int deb, int fin) {
		int posPivot = deb;
		int pivot = table[deb];
		for(int i=deb+1; i<=fin; i++) {
			if(table[i] < pivot) {
				posPivot++;
				swap(posPivot, i);
			}
		}
		swap(deb, posPivot);
		return posPivot;
	}

	private void quickSort(int deb, int fin) {
		if(deb < fin) {
			int positionPivot = partition(deb, fin);
			this.tasks.add(new Task(deb, positionPivot-1));
			this.tasks.add(new Task(positionPivot+1, fin));
			this.notifyBidon();
			/*quickSort(deb, positionPivot-1);
			quickSort(positionPivot+1, fin);*/
		}
		System.out.println(deb+" to "+fin+" done, still "+(this.nbToSort-1));
		this.oneMoreSorted();
	}

	private void swap(int a, int b) {
		int temp = table[a];
		table[a] = table[b];
		table[b] = temp;
	}
	
	public static void main(String[] args) throws InterruptedException {
		/*int[] tab = new int[] {4, 5, 8, 6, 6984, 51, 4, 2, 64, 6, 9};
		Sort sort = new Sort(tab);
		int[] result = sort.quickSort(4);
		for(int i=0; i<result.length; i++) {
			System.out.println(result[i]);
		}*/
		
		int[] tab = new int[10];
		for(int i=0; i<tab.length; i++) {
			tab[i] = (int)(Math.random()*10);
			System.out.print(tab[i]+" ");
		}
		System.out.println();
		SortParallel sort = new SortParallel(tab);
		long start = System.currentTimeMillis();
		int[] result = sort.quickSort(1);
		long end = System.currentTimeMillis();
		for(int i=0; i<result.length; i++) {
			System.out.print(result[i]+" ");
		}
		System.out.println();
		System.out.println("execution time: "+(end-start));
		for(int i=1; i<result.length; i++) {
			if(result[i-1] > result[i]) {
				System.out.println("sort incorrect at "+i+" ("+result[i-1]+" > "+result[i]+")!");
				break;
			}
		}
	}
}