package q2;

import java.util.ArrayList;
import java.util.List;

public class Matrix {
	private int[][] matrixA;
	private int[][] matrixB;
	private int lastElemCalculated;
	private int[][] mult;
	private final int totalNbElems;
	
	public class CalcThread extends Thread {
		@Override
		public void run() {
			int n;
			while((n = getElemToCalc()) != -1) {
				int i = n % matrixA.length;
				int j = n / matrixA.length;
				int val = 0;
				for(int k=0; k<matrixA.length; k++) {
					val += matrixA[k][j] * matrixB[i][k];
				}
				System.out.println("mult for ("+i+", "+j+") is "+val);
				setMultElem(i, j, val);
			}
		}
	}
	
	public Matrix(int[][] matrixA, int[][] matrixB) {
		if(matrixA.length != matrixB[0].length) {
			throw new IllegalArgumentException();
		}
		this.mult = new int[matrixA.length][matrixA.length];
		this.matrixA = matrixA;
		this.matrixB = matrixB;
		this.totalNbElems = this.matrixA.length * this.matrixA.length;
	}
	
	public synchronized void setMultElem(int i, int j, int val) {
		this.mult[i][j] = val;
	}
	
	public synchronized int getElemToCalc() {
		if(this.lastElemCalculated == this.totalNbElems) {
			return -1;
		}
		return this.lastElemCalculated++;
	}

	public int[][] calcMult(int nbThread) throws InterruptedException {
		this.lastElemCalculated = 0;
		List<CalcThread> threads = new ArrayList<CalcThread>();
		for(int i=0; i<nbThread; i++) {
			CalcThread thread = new CalcThread();
			threads.add(thread);
			thread.start();
		}
		for(CalcThread thread: threads) {
			thread.join();
		}
		return this.mult;
	}
	
	public static void main(String[] args) throws InterruptedException {
		int[][] m1 = new int[][] {
				new int[] {1, -1},
				new int[] {0, 3},
		};
		int[][] m2 = new int[][] {
				new int[] {3, 2},
				new int[] {1, 1},
		};
		System.out.println(m1[1][0]);
		Matrix matrix = new Matrix(m1, m2);
		int[][] mult = matrix.calcMult(2);
		for(int i=0; i<mult.length; i++) {
			for(int j=0; j<mult[0].length; j++) {
				System.out.print(mult[j][i]+" ");
			}
			System.out.println();
		}
	}
}
