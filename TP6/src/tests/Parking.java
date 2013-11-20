package tests;

import junit.framework.TestCase;

public class Parking extends TestCase {
	private static int nbCars;
	private static final int NB_PLACES_MAX = 10;
	
	public static class Gardien extends Thread {
		private String name;
		
		public Gardien(String name) {
			this.name = name;
		}
		
		@Override
		public void run() {
			for(int i=0; i<6; i++) {
				if(Parking.addCar()) {
					System.out.println(this.name+" just added car n"+Parking.getNbCars()+".");
				}
			}
		}
	}
	
	public static int getNbCars() {
		return nbCars;
	}
	
	public synchronized static boolean addCar() {
		if(!freeSpace()) {
			return false;
		}
		nbCars++;
		return true;
	}
	
	private static boolean freeSpace() {
		boolean freeSpace = nbCars < NB_PLACES_MAX;
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		return freeSpace;
	}
	
	public static void test() throws InterruptedException {
		Gardien gardienNord = new Gardien("Nord");
		Gardien gardienSud = new Gardien("Sud");
		gardienNord.start();
		Thread.sleep(50);
		gardienSud.start();
		gardienNord.join();
		gardienSud.join();
		System.out.println(Parking.getNbCars());
	}
}
