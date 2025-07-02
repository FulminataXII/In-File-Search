# search

An application that searches for a given string in all the text files in a given directory. 

## How To Use

Browse the directory to search in using the 'Browse' button or directly enter the path in 'Folder path' text field. 
Enter the string you want to search in Keyword text field. 
By default any partial matches for keyword are also detected. 
Example: Given "cat" as the keyword, it detects not only "cat" but also "catastrophic", "catch", "Muscat", etc . . . (case senstive is toggled, see below).
To disable partial matching, enter " keyword " for detecting only "keyword". 
Example: " cat " as keyword detects only "cat" (case senstive is toggled, see below). 
Toggle 'Case Sensitive?' to make the search case sensitive. Note: if Case Sensitive is not toggled then you must provide the keyword in lowercase. 
Toggle whichever punctuations to include in the search. If not toggled, they'll be ignored in the files. 
Example: Suppose ' isn't toggled. Then it detects "aren't", "arent", "a'rent", "a'r'ent", "a''''rent", etc. . . as matches for the keyword "arent" (case sentive is toggled). 
         Suppose ' is toggled. Then it detects "aren't" only for the keyword "aren't" (case senstive is toggled). 
