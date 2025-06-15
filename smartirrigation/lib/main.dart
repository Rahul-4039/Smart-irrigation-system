import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(); // Ensure Firebase is initialized
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: const MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  int _counter = 0; // Counter for any additional features
  bool On = false; // To toggle the light state
  final dbR = FirebaseDatabase.instance.ref(); // Reference to the Firebase Database

  void _incrementCounter() {
    setState(() {
      _counter++;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          crossAxisAlignment: CrossAxisAlignment.center,
          children: [
            On
                ? Icon(
              Icons.lightbulb,
              size: 100,
              color: Colors.amber,
            )
                : Icon(
              Icons.lightbulb_outline,
              size: 100,
            ),
            ElevatedButton(
              style: TextButton.styleFrom(
                backgroundColor: On ? Colors.green : Colors.white10,
              ),
              onPressed: () {
                dbR.child('light').set({'switch': !On}).then((_) {
                  setState(() {
                    On = !On; // Toggle the state
                  });
                }).catchError((error) {
                  // Handle error
                  print("Error writing to database: $error");
                });
              },
              child: On ? const Text('Motor On') : const Text('Motor Off'),
            ),
          ],
        ),
      ),
    );
  }
}
