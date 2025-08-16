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
  final TextEditingController _channelNameController = TextEditingController();
  final TextEditingController _userNameController = TextEditingController();
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
    _notifications.close();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(title: const Text('Notification Sender')),
        body: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.stretch,
            children: [
              TextField(
                controller: _userNameController,
                decoration: const InputDecoration(
                  border: OutlineInputBorder(),
                  labelText: 'User Name',
                ),
              ),
              const SizedBox(height: 20),
              TextField(
                controller: _channelNameController,
                decoration: const InputDecoration(
                  border: OutlineInputBorder(),
                  labelText: 'Channel Name',
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
              const SizedBox(height: 20),
              Expanded(
                child: Container(
                  padding: const EdgeInsets.all(8.0),
                  decoration: BoxDecoration(
                    border: Border.all(color: Colors.grey),
                    borderRadius: BorderRadius.circular(8.0),
                  ),
                  child: ListView.builder(
                    reverse: true,
                    itemCount: _logMessages.length,
                    itemBuilder: (context, index) {
                      return Text(_logMessages[index]);
                    },
                  ),
                ),
              ),
              const SizedBox(height: 20),
              ElevatedButton(
                onPressed: () {
                  if (_userNameController.text.isNotEmpty &&
                      _selectedDmImage != null) {
                    _notifications.send(
                      message: 'This is a direct message',
                      senderName: _userNameController.text,
                      channel: 'DM',
                      iconPath: 'assets/images/$_selectedDmImage',
                    );
                  }
                },
                child: const Text('Send DM'),
              ),
              const SizedBox(height: 10),
              ElevatedButton(
                onPressed: () {
                  if (_channelNameController.text.isNotEmpty &&
                      _selectedChannelImage != null) {
                    _notifications.send(
                      message: 'This is a channel message',
                      senderName: _userNameController.text,
                      channel: _channelNameController.text,
                      iconPath: 'assets/images/$_selectedChannelImage',
                    );
                  }
                },
                child: const Text('Send Channel Message'),
              ),
            ],
          ),
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
}
