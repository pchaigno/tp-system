package tests;

import junit.framework.TestCase;

public class ParkingExt extends TestCase {
	
	public static class ParkingNord {
		static int nbCarsNord;
		private static final int NB_PLACES_NORD = 5;

		public synchronized static boolean addCarNord() {
			if(!freeSpaceNord()) {
				return false;
			}
			nbCarsNord++;
			System.out.println("Gardien Sud welcomed car n°"+ParkingExt.getNbCars()+" at southern gate.");
			return true;
		}
		
		private static boolean freeSpaceNord() {
			boolean freeSpace = nbCarsNord < NB_PLACES_NORD;
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			return freeSpace;
		}
	}
	
	public static class ParkingSud {
		static int nbCarsSud;
		private static final int NB_PLACES_SUD = 13;

		public synchronized static boolean addCarSud() {
			if(!freeSpaceSud()) {
				return false;
			}
			nbCarsSud++;
			System.out.println("Gardien Sud welcomed car n°"+ParkingExt.getNbCars()+" at southern gate.");
			return true;
		}
		
		private static boolean freeSpaceSud() {
			boolean freeSpace = nbCarsSud < NB_PLACES_SUD;
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			return freeSpace;
		}
	}

	public static int getNbCars() {
		return ParkingNord.nbCarsNord + ParkingSud.nbCarsSud;
	}
	
	public static void test() throws InterruptedException {
		//GardienNord gardienNord = new GardienNord();
		//GardienSud gardienSud = new GardienSud();
		Thread gardienNord = new Thread() {
			@Override
			public void run() {
				while(ParkingNord.addCarNord());
				while(ParkingSud.addCarSud());
			}
		};
		Thread gardienSud = new Thread() {
			@Override
			public void run() {
				while(ParkingSud.addCarSud());
				while(ParkingNord.addCarNord());
			}
		};
		gardienNord.start();
		Thread.sleep(50);
		gardienSud.start();
		gardienNord.join();
		gardienSud.join();
		System.out.println(ParkingExt.getNbCars());
	}
}
