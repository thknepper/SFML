
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    // Defines PI
    const float PI = 3.14159f;

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML Pong");

    // Load the sounds used in the game
    sf::SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.LoadFromFile("resources/ball.wav"))
    {
        return EXIT_FAILURE;
    }
    sf::Sound ballSound(ballSoundBuffer);

    // Load the textures used in the game
    sf::Texture backgroundTexture, leftPaddleTexture, rightPaddleTexture, ballTexture;
    if (!backgroundTexture.LoadFromFile("resources/background.jpg")    ||
        !leftPaddleTexture.LoadFromFile("resources/paddle_left.png")   ||
        !rightPaddleTexture.LoadFromFile("resources/paddle_right.png") ||
        !ballTexture.LoadFromFile("resources/ball.png"))
    {
        return EXIT_FAILURE;
    }

    // Load the text font
    sf::Font font;
    if (!font.LoadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Initialize the end text
    sf::Text end;
    end.SetFont(font);
    end.SetCharacterSize(60);
    end.Move(150.f, 200.f);
    end.SetColor(sf::Color(50, 50, 250));

    // Create the sprites of the background, the paddles and the ball
    sf::Sprite background(backgroundTexture);
    sf::Sprite leftPaddle(leftPaddleTexture);
    sf::Sprite rightPaddle(rightPaddleTexture);
    sf::Sprite ball(ballTexture);

    leftPaddle.Move(10, (window.GetView().GetSize().y - leftPaddle.GetSize().y) / 2);
    rightPaddle.Move(window.GetView().GetSize().x - rightPaddle.GetSize().x - 10, (window.GetView().GetSize().y - rightPaddle.GetSize().y) / 2);
    ball.Move((window.GetView().GetSize().x - ball.GetSize().x) / 2, (window.GetView().GetSize().y - ball.GetSize().y) / 2);

    // Define the paddles properties
    sf::Clock AITimer;
    const float AITime     = 0.1f;
    float leftPaddleSpeed  = 400.f;
    float rightPaddleSpeed = 400.f;

    // Define the ball properties
    float ballSpeed = 400.f;
    float ballAngle;
    do
    {
        // Make sure the ball initial angle is not too much vertical
        ballAngle = std::rand() * 2 * PI / RAND_MAX;
    } while (std::abs(std::cos(ballAngle)) < 0.7f);

    bool isPlaying = true;
    while (window.IsOpened())
    {
        // Handle events
        sf::Event event;
        while (window.PollEvent(event))
        {
            // Window closed or escape key pressed : exit
            if ((event.Type == sf::Event::Closed) || 
               ((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Keyboard::Escape)))
            {
                window.Close();
                break;
            }
        }

        if (isPlaying)
        {
            // Move the player's paddle
            if (sf::Keyboard::IsKeyPressed(sf::Keyboard::Up) && (leftPaddle.GetPosition().y > 5.f))
                leftPaddle.Move(0.f, -leftPaddleSpeed * window.GetFrameTime() / 1000.f);
            if (sf::Keyboard::IsKeyPressed(sf::Keyboard::Down) && (leftPaddle.GetPosition().y < window.GetView().GetSize().y - leftPaddle.GetSize().y - 5.f))
                leftPaddle.Move(0.f, leftPaddleSpeed * window.GetFrameTime() / 1000.f);

            // Move the computer's paddle
            if (((rightPaddleSpeed < 0.f) && (rightPaddle.GetPosition().y > 5.f)) ||
                ((rightPaddleSpeed > 0.f) && (rightPaddle.GetPosition().y < window.GetView().GetSize().y - rightPaddle.GetSize().y - 5.f)))
            {
                rightPaddle.Move(0.f, rightPaddleSpeed * window.GetFrameTime() / 1000.f);
            }

            // Update the computer's paddle direction according to the ball position
            if (AITimer.GetElapsedTime() > AITime)
            {
                AITimer.Reset();
                if ((rightPaddleSpeed < 0) && (ball.GetPosition().y + ball.GetSize().y > rightPaddle.GetPosition().y + rightPaddle.GetSize().y))
                    rightPaddleSpeed = -rightPaddleSpeed;
                if ((rightPaddleSpeed > 0) && (ball.GetPosition().y < rightPaddle.GetPosition().y))
                    rightPaddleSpeed = -rightPaddleSpeed;
            }

            // Move the ball
            float factor = ballSpeed * window.GetFrameTime() / 1000.f;
                ball.Move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);

            // Check collisions between the ball and the screen
            if (ball.GetPosition().x < 0.f)
            {
                isPlaying = false;
                end.SetString("You lost !\n(press escape to exit)");
            }
            if (ball.GetPosition().x + ball.GetSize().x > window.GetView().GetSize().x)
            {
                isPlaying = false;
                end.SetString("You won !\n(press escape to exit)");
            }
            if (ball.GetPosition().y < 0.f)
            {
                ballSound.Play();
                ballAngle = -ballAngle;
                ball.SetY(0.1f);
            }
            if (ball.GetPosition().y + ball.GetSize().y > window.GetView().GetSize().y)
            {
                ballSound.Play();
                ballAngle = -ballAngle;
                ball.SetY(window.GetView().GetSize().y - ball.GetSize().y - 0.1f);
            }

            // Check the collisions between the ball and the paddles
            // Left Paddle
            if (ball.GetPosition().x < leftPaddle.GetPosition().x + leftPaddle.GetSize().x && 
                ball.GetPosition().x > leftPaddle.GetPosition().x + (leftPaddle.GetSize().x / 2.0f) &&
                ball.GetPosition().y + ball.GetSize().y >= leftPaddle.GetPosition().y &&
                ball.GetPosition().y <= leftPaddle.GetPosition().y + leftPaddle.GetSize().y)
            {
                ballSound.Play();
                ballAngle = PI - ballAngle;
                ball.SetX(leftPaddle.GetPosition().x + leftPaddle.GetSize().x + 0.1f);
            }

            // Right Paddle
            if (ball.GetPosition().x + ball.GetSize().x > rightPaddle.GetPosition().x &&
                ball.GetPosition().x + ball.GetSize().x < rightPaddle.GetPosition().x + (rightPaddle.GetSize().x / 2.0f) &&
                ball.GetPosition().y + ball.GetSize().y >= rightPaddle.GetPosition().y &&
                ball.GetPosition().y <= rightPaddle.GetPosition().y + rightPaddle.GetSize().y)
            {
                ballSound.Play();
                ballAngle = PI - ballAngle;
                ball.SetX(rightPaddle.GetPosition().x - ball.GetSize().x - 0.1f);
            }
        }

        // Clear the window
        window.Clear();

        // Draw the background, paddles and ball sprites
        window.Draw(background);
        window.Draw(leftPaddle);
        window.Draw(rightPaddle);
        window.Draw(ball);

        // If the game is over, display the end message
        if (!isPlaying)
            window.Draw(end);

        // Display things on screen
        window.Display();
    }

    return EXIT_SUCCESS;
}
