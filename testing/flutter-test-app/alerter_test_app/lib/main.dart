import 'dart:io';
import 'package:flutter/services.dart' show rootBundle;
import 'package:path_provider/path_provider.dart';
import 'package:alerter_test_app/notifications.dart';
import 'package:flutter/material.dart';

void main() {
  runApp(const MainApp());
}

class MainApp extends StatefulWidget {
  const MainApp({super.key});

  @override
  State<MainApp> createState() => _MainAppState();
}

class _MainAppState extends State<MainApp> {
  String? _selectedDmImage;
  String? _selectedChannelImage;
  final TextEditingController _channelNameController = TextEditingController(
    text: 'Default Channel',
  );
  final TextEditingController _userNameController = TextEditingController(
    text: 'Default User',
  );
  final TextEditingController _messageController = TextEditingController(
    text: 'Default message',
  );
  late final Notifications _notifications;
  final List<String> _logMessages = [];

  @override
  void initState() {
    super.initState();
    _notifications = Notifications(logger: _log);
  }

  void _log(String message) {
    setState(() {
      _logMessages.insert(0, message);
    });
  }

  @override
  void dispose() {
    _channelNameController.dispose();
    _userNameController.dispose();
    _messageController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(title: const Text('Notification Sender')),
        body: Row(
          children: [
            Expanded(
              flex: 2,
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.stretch,
                  children: [
                    TextField(
                      controller: _userNameController,
                      decoration: const InputDecoration(
                        border: OutlineInputBorder(),
                        labelText: 'User Name',
                        fillColor: Colors.white,
                        filled: true,
                      ),
                    ),
                    const SizedBox(height: 20),
                    TextField(
                      controller: _channelNameController,
                      decoration: const InputDecoration(
                        border: OutlineInputBorder(),
                        labelText: 'Channel Name',
                        fillColor: Colors.white,
                        filled: true,
                      ),
                    ),
                    const SizedBox(height: 20),
                    TextField(
                      controller: _messageController,
                      decoration: const InputDecoration(
                        border: OutlineInputBorder(),
                        labelText: 'Message',
                        fillColor: Colors.white,
                        filled: true,
                      ),
                    ),
                    const SizedBox(height: 20),
                    _buildImageRow(
                      title: 'Send DM',
                      images: ['elephant.png', 'frog.png', 'lion.png'],
                      selectedImage: _selectedDmImage,
                      onImageSelected: (image) {
                        setState(() {
                          _selectedDmImage = image;
                        });
                      },
                    ),
                    const SizedBox(height: 20),
                    _buildImageRow(
                      title: 'Send Channel Message',
                      images: ['medal.png', 'ping-pong.png', 'shoe.png'],
                      selectedImage: _selectedChannelImage,
                      onImageSelected: (image) {
                        setState(() {
                          _selectedChannelImage = image;
                        });
                      },
                    ),
                    const Spacer(),
                    ElevatedButton(
                      onPressed: () async {
                        if (_userNameController.text.isNotEmpty &&
                            _selectedDmImage != null) {
                          final absolutePath = await _getAbsoluteImagePath(
                            'assets/images/$_selectedDmImage',
                          );
                          _notifications.send({
                            'message': _messageController.text,
                            'senderName': _userNameController.text,
                            'channel': 'DM',
                            'iconPath': absolutePath,
                            'type': 'DM',
                            'timestamp': DateTime.now().toIso8601String(),
                          });
                        }
                      },
                      child: const Text('Send DM'),
                    ),
                    const SizedBox(height: 10),
                    ElevatedButton(
                      onPressed: () async {
                        if (_channelNameController.text.isNotEmpty &&
                            _selectedChannelImage != null) {
                          final absolutePath = await _getAbsoluteImagePath(
                            'assets/images/$_selectedChannelImage',
                          );
                          _notifications.send({
                            'message': _messageController.text,
                            'senderName': _userNameController.text,
                            'channel': _channelNameController.text,
                            'iconPath': absolutePath,
                            'type': 'channel',
                            'timestamp': DateTime.now().toIso8601String(),
                          });
                        }
                      },
                      child: const Text('Send Channel Message'),
                    ),
                  ],
                ),
              ),
            ),
            Expanded(
              flex: 1,
              child: Container(
                color: Colors.black,
                padding: const EdgeInsets.all(8.0),
                child: ListView.builder(
                  reverse: true,
                  itemCount: _logMessages.length,
                  itemBuilder: (context, index) {
                    return Text(
                      _logMessages[index],
                      style: const TextStyle(color: Colors.white),
                    );
                  },
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildImageRow({
    required String title,
    required List<String> images,
    required String? selectedImage,
    required ValueChanged<String> onImageSelected,
  }) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(title, style: Theme.of(context).textTheme.titleLarge),
        const SizedBox(height: 10),
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children:
              images
                  .map(
                    (image) => _buildImageSelector(
                      image,
                      selectedImage == image,
                      () => onImageSelected(image),
                    ),
                  )
                  .toList(),
        ),
      ],
    );
  }

  Widget _buildImageSelector(
    String imageName,
    bool isSelected,
    VoidCallback onTap,
  ) {
    return GestureDetector(
      onTap: onTap,
      child: Container(
        padding: const EdgeInsets.all(8),
        decoration: BoxDecoration(
          border: Border.all(
            color: isSelected ? Theme.of(context).primaryColor : Colors.grey,
            width: isSelected ? 3 : 1,
          ),
          borderRadius: BorderRadius.circular(8),
        ),
        child: Image.asset(
          'assets/images/$imageName',
          width: 50,
          height: 50,
          fit: BoxFit.cover,
        ),
      ),
    );
  }

  Future<String> _getAbsoluteImagePath(String assetPath) async {
    final byteData = await rootBundle.load(assetPath);
    final buffer = byteData.buffer;
    final tempDir = await getTemporaryDirectory();
    final fileName = assetPath.split('/').last;
    final file = File('${tempDir.path}/$fileName');
    await file.writeAsBytes(
      buffer.asUint8List(byteData.offsetInBytes, byteData.lengthInBytes),
    );
    return file.path;
  }
}
