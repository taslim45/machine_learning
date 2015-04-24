Text Classification Using Naive Bayes Multinomial Document Model


To understand what this approach does, see this video :
https://www.youtube.com/watch?v=km2LoOpdB3A

Reading material :
http://www.inf.ed.ac.uk/teaching/courses/inf2b/learnnotes/inf2b-learn-note07-2up.pdf


How to Run:
1. Open project in Eclipse or NetBeans.
2. Keep a spare copy of doc_db.db as backup.
3. Specify the location of doc_db.db in DBConnection.java
4. Although for each run the program clears all the data in the database,
prefer to use a copy of the backup database.


Caution:
The dataset contains over 7 hundred thousand words. The program runs on Java
and throws memory allocation error unless heap size is adjusted.
From Eclipse, go to Run->Run Configurations->Arguments Tab->VM arguments:
write "-Xmx512m" without quotes. Hit Apply and Run. 
This argument sets maximum heap size as 512m in the Java VM. In this case,
allocating more than 512m throws SQLite memory error.
For more information, see:
http://stackoverflow.com/questions/37335/how-to-deal-with-java-lang-outofmemoryerror-java-heap-space-error-64mb-heap