# In-File-Search
In File Search is a desktop application that allows users to search a given string in all the text-based files in a given directory and view the results per match and per file. 

This is made using Flutter and my own search algorithm (mostly KMP with some custom tweaks to allow more versatility like ignoring punctuations [see [How To Use](#how-to-use)]) written in C++ and built into a dll.
Flutter-dll integration is thanks to ffi package (https://pub.dev/packages/ffi). 

## How To Use

- Browse the directory to search in using the 'Browse' button or directly enter the path in 'Folder path' text field.  
- Enter the string you want to search in Keyword text field.  
- By default any partial matches for keyword are also detected.  
  _Example: Given "cat" as the keyword, it detects not only "cat" but also "catastrophic", "catch", "Muscat", etc . . . (case senstive is toggled, see below)._
- To disable partial matching, enter " keyword " for detecting only "keyword".  
  _Example: " cat " as keyword detects only "cat" (case senstive is toggled, see below)._
- Toggle 'Case Sensitive?' to make the search case sensitive. Note: if Case Sensitive is not toggled then you must provide the keyword in lowercase.  
- Toggle whichever punctuations to include in the search. If not toggled, they'll be ignored in the files.  
  _Example:<br/>
    Suppose  '  isn't toggled. Then it detects "aren't", "arent", "a'rent", "a'r'ent", "a''''rent", etc. . . as matches for the keyword "arent" (case sentive is toggled)._  
             _Now suppose  '  is toggled. Then it detects "aren't" only for the keyword "aren't" (case senstive is toggled)._ 

## Important Info
I've made this repo for academic purposes. Hence if you found this while searching for this or other like software, I'd suggest you to take a look at the excellent, InFile Seeker by Xtreme Lab (https://www.xtreme-lab.net/en/infiles.htm). It allows you to include or exclude custom extensions in your search and is also NOT SLOWER than mine ( yet ;) ).  
However, I don't think it allows you to ignore any punctuations (as of ... see the last modified date). 

## Some Comments
Thanks for your attention to my first ever repo!  <br/>

If you had the patience to look through at least some of my code and to comment any better implentation or about any issues in my code, Thank You! I'd love to learn somme good stuff from you. <br/>

I know there are probably a lot of things that you find which can be done better in this project. Taking your time to point out any one of them is also greatly appreciated. I will be updating the code, finetuning the features and resolving any bugs as per my convenience atleast till the foreseeable future or till I develop this to the extent I don't find any challenge or interest in going further, whichever is earlier. <br/>

Auf wiedersehen.
