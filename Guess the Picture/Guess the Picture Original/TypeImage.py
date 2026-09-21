import openai
openai.api_key = "sk-y7IDx7dwlBsUOdzSzlKogsYBkAQQZ5a_cXTgwDv-q6T3BlbkFJS4eIsmzucLpMGor92hI3Ww0IA4synKqLECs6i1f2cA"
class Image:
    def __init__(self, answer):
        self._correctanswer = answer
        self._hints = []
    def get_hint(self):
        chat_completion = openai.chat.completions.create(
            messages=[{
                "role": "user",
                "content": "My friends are playing guess the image. The correct answer is " + self._correctanswer + ". What hint would you give them? Please make the hint not too obvious. These are the hints that have already been given: " + str(self._hints) + ". Please just respond with the hint."
            }],
            model="gpt-4",
        )

        response_str = chat_completion.choices[0].message.content
#append the hint to self._hints, a list of the already given hints
        self._hints.append(response_str)
        response_str = response_str.strip("\"")
        return f"Hint: {response_str} "


    def get_answer_validation(self, guess):
        chat_completion = openai.chat.completions.create(
            messages=[{
                "role": "user",
                "content": "My friends are playing guess the image. The correct answer is " + self._correctanswer + ". Would " + str(guess) + " be a valid answer too? Please be lenient and please just respond yes or no."
            }],
            model="gpt-4",
        )
        response_str = chat_completion.choices[0].message.content
        if response_str.strip(".").lower() == "yes":
            return True
        return False


    def get_user_feedback(self, feedback):
        chat_completion = openai.chat.completions.create(
            messages=[{
                "role": "user",
                "content": "My friends were playing guess the image and the correct answer was " + self._correctanswer + "." + feedback
            }],
            model="gpt-4",
        )

        response_str = chat_completion.choices[0].message.content
        print(response_str)

