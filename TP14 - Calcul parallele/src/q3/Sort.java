package q3;

public class Sort {
	public static void triRapide(int tableau[])
	{
		int longueur=tableau.length;
		triRapide(tableau,0,longueur-1);
	}

	private static int partition(int tableau[],int deb,int fin)
	{
		int compt=deb;
		int pivot=tableau[deb];

		for(int i=deb+1;i<=fin;i++)
		{
			if (tableau[i]<pivot)
			{
				compt++;
				echanger(tableau,compt,i);
			}
		}
		echanger(tableau,deb,compt);
		return(compt);
	}

	private static void triRapide(int tableau[],int deb,int fin)
	{
		if(deb<fin)
		{
			int positionPivot=partition(tableau,deb,fin);
			triRapide(tableau,deb,positionPivot-1);
			triRapide(tableau,positionPivot+1,fin);
		}
	}

	private static void echanger(int[] table, int a, int b) {
		int temp = table[a];
		table[a] = table[b];
		table[b] = temp;
	}
	
	public static void main(String[] args) {
		int[] tab = new int[1000000];
		for(int i=0; i<tab.length; i++) {
			tab[i] = (int)(Math.random()*1000000);
			//System.out.print(tab[i]+" ");
		}
		System.out.println();
		long start = System.currentTimeMillis();
		Sort.triRapide(tab);
		long end = System.currentTimeMillis();
		for(int i=0; i<tab.length; i++) {
			System.out.print(tab[i]+" ");
		}
		System.out.println();
		System.out.println("execution time: "+(end-start));
		for(int i=1; i<tab.length; i++) {
			if(tab[i-1] > tab[i]) {
				System.out.println("sort incorrect at "+i+" ("+tab[i-1]+" > "+tab[i]+")!");
				break;
			}
		}
	}
}
