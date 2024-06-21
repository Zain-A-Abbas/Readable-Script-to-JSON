# Readable-Script-to-JSON
This project converts dialogue files from a naturalistic, human-readable format to a JSON format for use in games.

Script Style Guide for general scripts
The first line of a script file is the output file name. Non alphanumeric characters are stripped.
Any lines beginning with any or no text, followed by a colon, followed by more text represents a line of dialogue, with the text before the colon representing the character name and the text after being the actual dialogue. No text preceding the colon will register as a line without a speaker.
Any line that begins with ‘>’ e.g. >CutsceneA, represents a dialogue group. Dialogue groups are used in-engine for the programmer, and generally, dialogue in one interaction or cutscene should be in the same group. If a line of dialogue is placed before a single group is defined, the program will return an error.
Any line beginning with / represents a comment for the writer/translator
Any line not following these, or any dialogue not under a dialogue group will cause the software to return an error and fail to convert.
