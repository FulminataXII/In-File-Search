import 'package:flutter/material.dart';
import 'package:file_picker/file_picker.dart';
import 'search_ffi.dart';
import 'dart:convert';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Search GUI',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: const Color.fromARGB(255, 102, 205, 253)),
        useMaterial3: true,
      ),
      home: const SearchHomePage(),
    );
  }
}

class SearchHomePage extends StatefulWidget {
  const SearchHomePage({super.key});

  @override
  State<SearchHomePage> createState() => _SearchHomePageState();
}

class _SearchHomePageState extends State<SearchHomePage> {
  final TextEditingController _folderController = TextEditingController();
  final TextEditingController _keywordController = TextEditingController();
  bool _isSearching = false;
  final List<String> _punctuationOptions = [',', '.', '?', '!', '-', '_', "'", '"', ':', ';'];
  final Set<String> _selectedPunctuations = {};
  bool _caseSensitive = false;

  Future<void> _pickFolder() async {
    String? selectedDirectory = await FilePicker.platform.getDirectoryPath();
    if (selectedDirectory != null) {
      setState(() {
        _folderController.text = '$selectedDirectory\\';
      });
    }
  }

  void _onSearch() async {
    setState(() {
      _isSearching = true;
    });
    
    String dirPath = _folderController.text;
    if (!dirPath.endsWith('\\')) {
      dirPath += '\\';
    }
    
    final puncts = _selectedPunctuations.join();
    
    final jsonString = await Future(() => SearchFFI.search(
      pattern: _keywordController.text,
      directory: dirPath,
      caseSensitive: _caseSensitive,
      puncts: puncts,
    ));
    
    final List<dynamic> jsonResults = jsonDecode(jsonString);
    final results = jsonResults.map((file) => SearchResult(
      filePath: file['filePath'],
      matches: (file['matches'] as List).map((m) => KeywordMatch(
        line: m['line'],
        position: m['position'],
        lineText: m['lineText'],
      )).toList(),
    )).toList();
    
    if (results.isNotEmpty) {
      Navigator.push(
        context,
        MaterialPageRoute(
          builder: (context) => SearchResultsPage(
            results: results,
            keyword: _keywordController.text,
          ),
        ),
      );
    } else {
      
      showDialog(
        context: context,
        builder: (context) => AlertDialog(
          title: const Text('No Results'),
          content: const Text('No matches found.'),
          actions: [
            TextButton(
              onPressed: () => Navigator.of(context).pop(),
              child: const Text('Okay.'),
            ),
          ],
        ),
      );
    }
    setState(() {
      _isSearching = false;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('InFile Search.')),
      body: Padding(
        padding: const EdgeInsets.all(24.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              children: [
                Expanded(
                  child: TextField(
                    controller: _folderController,
                    decoration: const InputDecoration(
                      labelText: 'Folder Path : ',
                      border: OutlineInputBorder(),
                    ),
                  ),
                ),
                const SizedBox(width: 8),
                ElevatedButton.icon(
                  onPressed: _pickFolder,
                  icon: const Icon(Icons.folder),
                  label: const Text('Browse.'),
                ),
              ],
            ),
            const SizedBox(height: 16),
            TextField(
              controller: _keywordController,
              decoration: const InputDecoration(
                labelText: 'Keyword : ',
                border: OutlineInputBorder(),
              ),
            ),
            const SizedBox(height: 16),
            
            Row(
              children: [
                Switch(
                  value: _caseSensitive,
                  onChanged: (v) => setState(() => _caseSensitive = v),
                ),
                const SizedBox(width: 8),
                const Text('Case Sensitive? '),
              ],
            ),
            const SizedBox(height: 16),
            
            Wrap(
              spacing: 8,
              runSpacing: 8,
              children: _punctuationOptions.map((p) => FilterChip(
                label: Text(p),
                selected: _selectedPunctuations.contains(p),
                onSelected: (selected) {
                  setState(() {
                    if (selected) {
                      _selectedPunctuations.add(p);
                    } else {
                      _selectedPunctuations.remove(p);
                    }
                  });
                },
              )).toList(),
            ),
            const SizedBox(height: 24),
            Center(
              child: ElevatedButton.icon(
                onPressed: _isSearching ? null : _onSearch,
                icon: const Icon(Icons.search),
                label: _isSearching ? const Text('Searching...') : const Text('Search'),
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class SearchResultsPage extends StatefulWidget {
  final List<SearchResult> results;
  final String keyword;
  const SearchResultsPage({super.key, required this.results, required this.keyword});

  @override
  State<SearchResultsPage> createState() => _SearchResultsPageState();
}

class _SearchResultsPageState extends State<SearchResultsPage> {
  int _selectedFileIdx = 0;
  int _selectedMatchIdx = 0;

  @override
  Widget build(BuildContext context) {
    final selectedResult = widget.results[_selectedFileIdx];
    final selectedMatch = selectedResult.matches[_selectedMatchIdx];
    return Scaffold(
      appBar: AppBar(title: const Text('Results')),
      body: Row(
        children: [
          // File list
          Container(
            width: 220,
            child: ListView.builder(
              itemCount: widget.results.length,
              itemBuilder: (context, idx) {
                return ListTile(
                  title: Text(widget.results[idx].filePath),
                  selectedTileColor: Colors.yellow[100],
                  selected: idx == _selectedFileIdx,
                  hoverColor: idx == _selectedFileIdx ? null : Colors.lightBlueAccent,
                  onTap: () {
                    setState(() {
                      _selectedFileIdx = idx;
                      _selectedMatchIdx = 0;
                    });
                  },
                );
              },
            ),
          ),
          Expanded(
            child: Padding(
              padding: const EdgeInsets.all(24.0),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(selectedResult.filePath, style: Theme.of(context).textTheme.titleLarge),
                  const SizedBox(height: 16),
                  Expanded(
                    child: ListView(
                      children: selectedResult.matches.map((match) {
                        final isSelected = match == selectedMatch;
                        return Container(
                          color: isSelected ? Colors.yellow[100] : null,
                          child: ListTile(
                            title: _highlightKeyword(match.lineText, widget.keyword, position: match.position),
                            subtitle: Text('Line ${match.line}, Pos ${match.position}'),
                            hoverColor: isSelected ? null : Colors.lightBlueAccent,
                            onTap: () => setState(() => isSelected ? null : _selectedMatchIdx = selectedResult.matches.indexOf(match)),
                          ),
                        );
                      }).toList(),
                    ),
                  ),
                  Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      ElevatedButton(
                        onPressed: _selectedMatchIdx > 0
                            ? () => setState(() => _selectedMatchIdx--)
                            : null,
                        child: const Text('Previous.'),
                      ),
                      const SizedBox(width: 16),
                      ElevatedButton(
                        onPressed: _selectedMatchIdx < selectedResult.matches.length - 1
                            ? () => setState(() => _selectedMatchIdx++)
                            : null,
                        child: const Text('Next.'),
                      ),
                    ],
                  ),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }

  Widget _highlightKeyword(String text, String keyword, {int? position}) {
    if (keyword.isEmpty || position == null || position < 0 || position >= text.length) {
      return Text(text);
    }
    
    int left = position;
    while (left > 0 && text[left - 1].trim().isNotEmpty) {
      left--;
    }
    
    int right = position;
    while (right < text.length && text[right].trim().isNotEmpty) {
      right++;
    }
    
    List<TextSpan> spans = [];
    if (left > 0) {
      spans.add(TextSpan(text: text.substring(0, left)));
    }
    spans.add(TextSpan(
      text: text.substring(left, right),
      style: const TextStyle(backgroundColor: Colors.yellow, fontWeight: FontWeight.bold),
    ));
    if (right < text.length) {
      spans.add(TextSpan(text: text.substring(right)));
    }
    return RichText(text: TextSpan(style: const TextStyle(color: Colors.black), children: spans));
  }
}

class SearchResult {
  final String filePath;
  final List<KeywordMatch> matches;
  SearchResult({required this.filePath, required this.matches});
}

class KeywordMatch {
  final int line;
  final int position;
  final String lineText;
  KeywordMatch({required this.line, required this.position, required this.lineText});
}
