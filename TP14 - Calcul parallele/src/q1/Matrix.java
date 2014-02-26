package q1;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;


public class Matrix {
	private int[][] matrix;
	private int lastColumnCalculated;
	private Vector<Integer> sums;
	
	public class CalcThread extends Thread {
		@Override
		public void run() {
			int i;
			while((i = getColumnToCalc()) != -1) {
				int sum = 0;
				for(int j=0; j<matrix[i].length; j++) {
					sum += matrix[i][j];
				}
				System.out.println("sum for column "+i+" is "+sum);
				sums.set(i, sum);
			}
		}
	}
	
	public Matrix(int[][] matrix) {
		this.matrix = matrix;
	}
	
	public synchronized int getColumnToCalc() {
		if(this.lastColumnCalculated == this.matrix.length) {
			return -1;
		}
		return this.lastColumnCalculated++;
	}

	public Vector<Integer> calcSumColumns(int nbThread) throws InterruptedException {
		this.lastColumnCalculated = 0;
		this.sums = new Vector<Integer>();
		this.sums.setSize(this.matrix.length);
		List<CalcThread> threads = new ArrayList<CalcThread>();
		for(int i=0; i<nbThread; i++) {
			CalcThread thread = new CalcThread();
			threads.add(thread);
			thread.start();
		}
		for(CalcThread thread: threads) {
			thread.join();
		}
		return sums;
	}
	
	public static void main(String[] args) throws InterruptedException {
		int[][] m = new int[][]{
				new int[] {1, 2, 3, 4, 5},
				new int[] {5, 4, 3, 2, 1},
				new int[] {1, 1, 1, 1, 1},
				new int[] {5, 5, 5, 5, 5},
				new int[] {9, 8, 7, 6, 5},
				new int[] {0, 0, 0, 0, 0},
		};
		Matrix matrix = new Matrix(m);
		Vector<Integer> sums = matrix.calcSumColumns(8);
		for(int i=0; i<m.length; i++) {
			int sum = 0;
			for(int j=0; j<m[i].length; j++) {
				sum += m[i][j];
			}
			System.out.println(sums.get(i)+" - "+sum);
		}
	}
}
