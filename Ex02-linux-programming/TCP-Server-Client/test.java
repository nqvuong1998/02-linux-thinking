import java.io.*; 
import java.text.*; 
import java.util.*; 
public class Main{
    public static void main(String[] args) {
        Random rd = new Random();  
        int sizeArr = 100 + rd.nextInt(1000 - 100 + 1); 
        //System.out.println("Số vừa được sinh ra là " + sizeArr);
             
        int[] intArr = new int[sizeArr];
        for(int i = 0;i<sizeArr;i++){
            intArr[i] = 1 + rd.nextInt(Integer.MAX_VALUE - 1 + 1);
        }
    }
}