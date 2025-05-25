# LORM Glove System

## Overview
The LORM Glove System is a web application designed to facilitate communication for deafblind individuals using the LORM tactile alphabet. This project provides a user-friendly interface for real-time tactile communication, allowing users to connect and interact seamlessly.

## Project Structure
```
lorm-app
├── public
│   └── index.php
├── src
│   ├── assets
│   │   ├── css
│   │   │   └── style.css
│   │   └── js
│   │       └── main.js
│   ├── includes
│   │   ├── config.php
│   │   ├── header.php
│   │   ├── footer.php
│   │   └── sidebar.php
│   ├── pages
│   │   ├── home.php
│   │   ├── messenger.php
│   │   └── team.php
│   └── templates
│       └── layout.php
├── .htaccess
├── composer.json
└── README.md
```

## Setup Instructions
1. Clone the repository to your local machine.
2. Navigate to the project directory.
3. Install the required dependencies using Composer:
   ```
   composer install
   ```
4. Configure your database settings in `src/includes/config.php`.
5. Start your local server (e.g., XAMPP, MAMP) and navigate to `http://localhost/lorm-app/public/index.php`.

## Usage
- **Home Page**: Provides an overview of the LORM Glove System and its features.
- **Messenger**: Allows users to send and receive messages using the LORM Glove.
- **Our Team**: Displays information about the team members involved in the project.

## Contributing
Contributions are welcome! Please submit a pull request or open an issue for any suggestions or improvements.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.