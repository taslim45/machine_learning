package utils;

import java.util.TreeSet;

public class WordUtils {

	static TreeSet<String> prepostions;
	
	public static void listPrepositions()
	{
		prepostions = new TreeSet<String>();
		prepostions.add("a");
		prepostions.add("an");
		prepostions.add("and");
		prepostions.add("any");
		prepostions.add("about");
		prepostions.add("at");
		prepostions.add("after");
		prepostions.add("are");
		prepostions.add("as");
		prepostions.add("be");
		prepostions.add("been");
		prepostions.add("by");
		prepostions.add("before");
		prepostions.add("during");
		prepostions.add("for");
		prepostions.add("from");
		prepostions.add("have");
		prepostions.add("has");
		prepostions.add("in");
		prepostions.add("it");
		prepostions.add("its");
		prepostions.add("is");
		prepostions.add("into");
		prepostions.add("like");
		prepostions.add("of");
		prepostions.add("off");
		prepostions.add("on");
		prepostions.add("out");
		prepostions.add("since");
		prepostions.add("so");
		prepostions.add("some");
		prepostions.add("though");
		prepostions.add("through");
		prepostions.add("the");
		prepostions.add("there");
		prepostions.add("to");
		prepostions.add("them");
		prepostions.add("their");
		prepostions.add("this");
		prepostions.add("up");
		prepostions.add("with");
		prepostions.add("without");
		prepostions.add("was");
	}
	public static boolean isPreposition(String word)
	{
		return prepostions.contains(word);
	}
	public static String cleanWord(String word)
	{
		String str = "";
		int wlen = word.length();
		if(wlen == 1 && word.charAt(0) == '-') return "";
		char first = word.charAt(0);
		char last = word.charAt(wlen-1);
		if(word.startsWith("&lt;") || word.contains("&lt;"))
		{
			if(last!=';' && first!='&') str = word.substring(5,wlen-1);
			else if(first!='&') str = word.substring(5,wlen);
			else if (last!=';') str = word.substring(4, wlen-1);
			else str = word.substring(4, wlen);
			wlen = str.length();
			if(str.endsWith("&gt;"))
			{
				return str.substring(0, wlen-4);
			}
		}
		else if(first>=48 && first<=57 && last>=48 && last<=57) return word;
		else
		{
			int i,j,stagnant;
			i = 0;
			j = wlen-1;
			stagnant = 0;
			while(i<j)
			{
				char ci = word.charAt(i);
				char cj= word.charAt(j);
				if((ci>=97 && ci<=122) || (ci>=48 && ci<=57)) stagnant++;
				else i++;
				if((cj>=97 && cj<=122) || (cj>=48 && cj<=57)) stagnant++;
				else j--;
				if(stagnant == 2) break;
				else stagnant = 0;
			}
			return word.substring(i,j+1);
		}
		return "";
	}
}
