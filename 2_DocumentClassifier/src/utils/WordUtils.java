package utils;

public class WordUtils {

	
	public static String cleanWord(String word)
	{
		String str = "";
		int wlen = word.length();
		if(wlen == 1 && word.charAt(0) == '-') return "";
		char first = word.charAt(0);
		char last = word.charAt(wlen-1);
		if(word.startsWith("&lt;"))
		{
			str = word.substring(4, wlen);
			wlen = str.length();
			if(str.endsWith("&gt;"))
			{
				return str.substring(0, wlen-4);
			}
		}
		else if(first>=48 && last<=57) return word;
		else
		{
			int i,j,stagnant;
			i = 0;
			j = wlen-1;
			stagnant = 0;
			while(i<j)
			{
				if(word.charAt(i)<97 || word.charAt(i)>122) i++;
				else stagnant++;
				if(word.charAt(j)<97 || word.charAt(j)>122) j--;
				else stagnant++;
				if(stagnant == 2) break;
				else stagnant = 0;
			}
			return word.substring(i,j+1);
		}
		return "";
	}
}
