/*
 * state_machine.c
 *
 *  Created on: Oct 30, 2024
 *      Author: Chanchai.Th
 */
#include "state_machine.h"
#include "storage.h"

const char *stateStrings[] = { "IDLE", "CLOSE", "OPEN", "CUSTOM" };

uint32_t openPattern[3];
uint32_t closePattern[3];

State currentState = STATE_IDLE;
CustomState customState = INIT;

uint32_t countTime = 0;
TIM_HandleTypeDef *htim;

int statusoptton = 0;
int ledBlinkCount = 0;
char msg[100];
char msg2[100];
bool isPressing = false;
bool isPressing2 = false;

uint32_t timeopen[1];

uint32_t pressStartTime = 0;
uint16_t counttime3s = 0;

/*
 * StateMc_Init
 *
 * Initializes the state machine by loading sensor patterns and timeout values
 * from flash memory. It also validates the read patterns and ensures they are within
 * the expected range. If any patterns are invalid, default values are set and written back
 * to flash memory.
 */
void StateMc_Init(TIM_HandleTypeDef *h) {
	htim = h;

	// Erase flash memory (do not un-comment)
	// EraseStorage();

	// Read sensor pattern from flash memory
	ReadOpenPattern(openPattern);
	ReadClosePattern(closePattern);
	ReadTimeOutOpen(timeopen);

	if (!(openPattern[0] >= 1 && openPattern[0] <= 3) || !(openPattern[1] >= 1 && openPattern[1] <= 3) || !(openPattern[2] >= 1 && openPattern[2] <= 3)) {
		openPattern[0] = 1;
		openPattern[1] = 2;
		openPattern[2] = 3;

		WriteOpenPattern(openPattern);
	}

	if (!(closePattern[0] >= 1 && closePattern[0] <= 3) || !(closePattern[1] >= 1 && closePattern[1] <= 3) || !(closePattern[2] >= 1 && closePattern[2] <= 3)) {
		closePattern[0] = 3;
		closePattern[1] = 2;
		closePattern[2] = 1;

		WriteClosePattern(closePattern);
	}
}

/*
 * stateRunning
 *
 * This function manages the different states of the system (IDLE, CLOSE, OPEN, CONFIG).
 * It displays messages on the OLED screen and controls various hardware components (e.g., LEDs, buzzer, sensors).
 * It handles timeouts, button presses, and transitions between states based on sensor input and timers.
 */
void stateRunning() {
// Display State on the screen
	sprintf(msg, "%s", stateStrings[currentState]);

	switch (currentState) {
	case STATE_IDLE:
		Led_Show(true, false, true);

		if (ReadOpttonValue(1) || ReadOpttonValue(2) || ReadOpttonValue(3)) {
			if (countTime == 0) {
				countTime = HAL_GetTick();
			}

			uint32_t remainTime = HAL_GetTick() - countTime;

			if (ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {

				if (remainTime > USER_TIMEOUT_3S) {
					goReadyState();
				} else {
					sprintf(msg, "Ready in %u..        ", (uint16_t) ((USER_TIMEOUT_3S - remainTime + 999) / 1000));
				}
			} else if (ReadOpttonValue(1) && ReadOpttonValue(2) && ReadOpttonValue(3)) {

				if (remainTime > USER_TIMEOUT_3S) {
					goCustomState();
				} else {
					sprintf(msg, "Customize in %u..    ", (uint16_t) ((USER_TIMEOUT_3S - remainTime + 999) / 1000));
				}
			} else {
				if (remainTime > USER_TIMEOUT_1S) {

					countTime = 0;
				}
			}
		} else {
			countTime = 0;
			sprintf(msg, "                    ");
			Oled_ShowMessage_sm(msg, 0, 53);
		}
		break;

	case STATE_CLOSE:
		showLedSensorIndex(statusoptton, PATTERN_OPEN);

		sprintf(msg, "  %d %d %d to Open", (int) openPattern[0], (int) openPattern[1], (int) openPattern[2]);
		Oled_ShowMessage_sm(msg, 0, 27);

		if (ReadOpttonValue(1) || ReadOpttonValue(2) || ReadOpttonValue(3)) {
			sprintf(msg, "                    ");
			Oled_ShowMessage_sm(msg, 0, 53);

			switch (statusoptton) {
			case 0:
				while (isPressing) {
					if (!(ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3))) {
						isPressing = false;
					}
				}

				if (readSensorIndex(statusoptton, PATTERN_OPEN)) {
					Buzzer_OK();
					statusoptton = 1;
					isPressing = true;
				} else {

				}

				break;

			case 1:
				while (isPressing) {
					if (!readSensorIndex(statusoptton - 1, PATTERN_OPEN)) {
						isPressing = false;
					}
				}

				if (readSensorIndex(statusoptton, PATTERN_OPEN)) {
					Buzzer_OK();
					statusoptton = 2;
					isPressing = true;
				} else {

				}

				break;

			case 2:
				while (isPressing) {
					if (!readSensorIndex(statusoptton - 1, PATTERN_OPEN)) {
						isPressing = false;
					}
				}

				if (readSensorIndex(statusoptton, PATTERN_OPEN)) {
					goOpenState();
				} else {

				}

				break;

			default:
				statusoptton = 0;
				break;
			}

			countTime = 0;
		} else {
			if (countTime == 0) {
				countTime = HAL_GetTick();
			}

			uint32_t remainTime = HAL_GetTick() - countTime;

			if (remainTime > USER_TIMEOUT_30S) {
				goIdleState();
			} else {
				sprintf(msg, "Idle in %u..         ", (uint16_t) ((USER_TIMEOUT_30S - remainTime + 999) / 1000));
				Oled_ShowMessage_sm(msg, 0, 53);
			}

			isPressing = false;
		}

		break;

	case STATE_OPEN:
		showLedSensorIndex(statusoptton, PATTERN_CLOSE);

		sprintf(msg, "  %d %d %d to Close", (int) closePattern[0], (int) closePattern[1], (int) closePattern[2]);
		Oled_ShowMessage_sm(msg, 0, 27);

		if (ReadOpttonValue(1) || ReadOpttonValue(2) || ReadOpttonValue(3)) {
			sprintf(msg, "                    ");
			Oled_ShowMessage_sm(msg, 0, 53);

			switch (statusoptton) {
			case 0:
				if (readSensorIndex(statusoptton, PATTERN_CLOSE)) {
					Buzzer_OK();
					statusoptton = 1;
					isPressing = true;
				} else {

				}

				break;

			case 1:
				while (isPressing) {
					if (!readSensorIndex(statusoptton - 1, PATTERN_CLOSE)) {
						isPressing = false;
					}
				}

				if (readSensorIndex(statusoptton, PATTERN_CLOSE)) {
					Buzzer_OK();
					statusoptton = 2;
					isPressing = true;
				} else {

				}

				break;

			case 2:
				while (isPressing) {
					if (!readSensorIndex(statusoptton - 1, PATTERN_CLOSE)) {
						isPressing = false;
					}
				}

				if (readSensorIndex(statusoptton, PATTERN_CLOSE)) {
					goCloseState();
				} else {

				}

				break;

			default:
				statusoptton = 0;
				break;
			}

			countTime = 0;
		} else {
			if (countTime == 0) {
				countTime = HAL_GetTick();
			}

			uint32_t remainTime = HAL_GetTick() - countTime;

			if (timeopen[0] == 1) {
				if (remainTime > USER_TIMEOUT_30S) {
					goCloseState();
				} else {
					sprintf(msg, "Close in %u..        ", (uint16_t) ((USER_TIMEOUT_30S - remainTime + 999) / 1000));
					Oled_ShowMessage_sm(msg, 0, 53);
				}
			}
			isPressing = false;
		}

		break;

	case STATE_CONFIG:

		stateCustomTask();

		break;

	default:
		goIdleState();

		break;
	}
}

/*
 * stateCustomTask
 *
 * This function handles the custom configuration task for setting the open and close patterns, as well as the open time.
 * It uses an FSM (finite state machine) to manage the states and transitions between them, displaying messages and handling user input via button presses.
 */
void stateCustomTask() {
	bool customRunning = true;

	countTime = 0;

	Oled_Clear();

	customState = INIT;

	while (customRunning) {
		switch (customState) {
		case INIT:
			Oled_ShowMessage_sm("Perform operation?", 0, 0);
			Oled_ShowMessage_sm("Yes(1)       No(3)", 0, 53);

			if (countTime == 0) {
				countTime = HAL_GetTick();
			}

			uint32_t remainTime = HAL_GetTick() - countTime;

			sprintf(msg, "(Idle in %u..)        ", (uint16_t) ((USER_TIMEOUT_10S - remainTime + 999) / 1000));
			Oled_ShowMessage_sm(msg, 0, 16);

			if (remainTime > USER_TIMEOUT_10S) {
				customState = EXIT;
			}

			buttonStateHandler(SETTING, EXIT);

			break;
		case SETTING:
			Oled_ShowMessage_sm("SETTING", 0, 0);
			Oled_ShowMessage_sm("Modify pattern(1) ", 0, 16);
			Oled_ShowMessage_sm("Set OPEN time(2) ", 0, 34);
			Oled_ShowMessage_sm("Back to IDLE(3)", 0, 53);

			buttonStateHandler2(OPEN_BEGIN, OPEN_TIME, EXIT);

			break;

		case OPEN_TIME:
			Oled_ShowMessage_sm("Set OPEN TIME?", 0, 0);
			Oled_ShowMessage_sm("Yes(1)       No(3)", 0, 53);

			buttonStateHandler(OPEN_TIME_SELECT, SETTING);

			break;
		case OPEN_TIME_SELECT:
			Oled_ShowMessage_sm("Choose OPEN TIME?", 0, 0);
			Oled_ShowMessage_sm("Init 30 s (1) ", 0, 26);
			Oled_ShowMessage_sm("No Timeout (3) ", 0, 53);

			buttonStateHandler(OPEN_TIME_SELECT_INIT, OPEN_TIME_SELECT_NOTIME);

			break;

		case OPEN_TIME_SELECT_INIT:
			Oled_ShowMessage_sm("INIT", 0, 0);
			Oled_ShowMessage_sm("Ok(1)    Back(3)", 0, 53);
			sprintf(msg, "(Time: %d)", USER_TIMEOUT_30S / 1000);
			Oled_ShowMessage_sm(msg, 0, 16);
			timeopen[0] = 1;
			WriteTimeOutOpen(timeopen);
			buttonStateHandler(DONE, OPEN_TIME_SELECT);

			break;
		case OPEN_TIME_SELECT_NOTIME:
			Oled_ShowMessage_sm("NO Timeout", 0, 0);
			Oled_ShowMessage_sm("Ok(1)    Back(3)", 0, 53);
			timeopen[0] = 0;
			WriteTimeOutOpen(timeopen);
			buttonStateHandler(DONE, OPEN_TIME_SELECT);

			break;

		case OPEN_BEGIN:
			Oled_ShowMessage_sm("Modify OPEN?", 0, 0);
			Oled_ShowMessage_sm("Yes(1)       No(3)", 0, 53);

			strcpy(msg, "");
			sprintf(msg, "(Current: %d %d %d)", (int) openPattern[0], (int) openPattern[1], (int) openPattern[2]);

			buttonStateHandler(OPEN_SELECT, CLOSE_BEGIN);

			break;
		case OPEN_SELECT:


			strcpy(msg, "");
			sprintf(msg, "(Current: %d %d %d)", (int) openPattern[0], (int) openPattern[1], (int) openPattern[2]);

			buttonStateHandler(OPEN_DEFAULT, OPEN_CUSTOM);

			break;
		case OPEN_DEFAULT:
			setDefaultPattern(PATTERN_OPEN);
			customState = CLOSE_BEGIN;

			break;
		case OPEN_CUSTOM:
			setCustomPattern(PATTERN_OPEN);
			//customState = CLOSE_BEGIN;

			break;
		case CLOSE_BEGIN:

			strcpy(msg, "");
			sprintf(msg, "(Current: %d %d %d)", (int) closePattern[0], (int) closePattern[1], (int) closePattern[2]);

			buttonStateHandler(CLOSE_SELECT, SETTING);

			break;
		case CLOSE_SELECT:

			strcpy(msg, "");

			buttonStateHandler(CLOSE_DEFAULT, CLOSE_CUSTOM);

			break;
		case CLOSE_DEFAULT:
			setDefaultPattern(PATTERN_CLOSE);
			customState = DONE;

			break;
		case CLOSE_CUSTOM:
			setCustomPattern(PATTERN_CLOSE);
			//customState = DONE;

			break;
		case DONE:
			HAL_Delay(1000);
			customState = SETTING;

			break;
		case EXIT:
			customRunning = false;

			break;
		default:
			customState = INIT;

			break;
		}
	}

	goIdleState();
}

/*
 * buttonStateHandler
 *
 * This function handles button presses and transitions the custom state based on the button combinations.
 * - It reads the state of three buttons and uses the values to decide on transitions.
 * - It includes buzzer feedback for valid (OK) and invalid (NG) button presses.
 *
 * Parameters:
 *   - state_yes: The custom state to transition to when a valid button press is detected for 'Yes'.
 *   - state_no: The custom state to transition to when a valid button press is detected for 'No'.
 */
void buttonStateHandler(CustomState state_yes, CustomState state_no) {
	if (ReadOpttonValue(1) && !ReadOpttonValue(2) && !ReadOpttonValue(3)) {
		HAL_Delay(500);
		if (ReadOpttonValue(1) && !ReadOpttonValue(2) && !ReadOpttonValue(3)) {
			customState = state_yes;
		}
	} else if (!ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {
		HAL_Delay(500);
		if (!ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {
			customState = state_no;
		}
	} else if (!ReadOpttonValue(1) && ReadOpttonValue(2) && !ReadOpttonValue(3)) {
		HAL_Delay(500);

	}
}

/*
 * buttonStateHandler2
 *
 * This function handles button presses and transitions to one of three custom states
 * based on the button inputs. It provides buzzer feedback for valid presses.
 *
 * Parameters:
 *   - state_1: The custom state to transition to when button 1 is pressed.
 *   - state_2: The custom state to transition to when button 2 is pressed.
 *   - state_3: The custom state to transition to when button 3 is pressed.
 */
void buttonStateHandler2(CustomState state_1, CustomState state_2, CustomState state_3) {
	if (ReadOpttonValue(1) && !ReadOpttonValue(2) && !ReadOpttonValue(3)) {
		HAL_Delay(500);
		if (ReadOpttonValue(1) && !ReadOpttonValue(2) && !ReadOpttonValue(3)) {
			customState = state_1;
		}
	} else if (!ReadOpttonValue(1) && ReadOpttonValue(2) && !ReadOpttonValue(3)) {
		HAL_Delay(500);
		if (!ReadOpttonValue(1) && ReadOpttonValue(2) && !ReadOpttonValue(3)) {
			customState = state_2;
		}
	} else if (!ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {
		HAL_Delay(500);
		if (!ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {
			customState = state_3;
		}
	}
}

/*
 * goIdleState
 *
 * This function returns the system to an idle state by performing a series of actions,
 * including stopping the servo, playing a success sound, clearing the LED,
 * clearing the OLED display, and resetting various system variables.
 */
void goIdleState() {
	Led_Clear();
	statusoptton = 0;
	countTime = 0;
	currentState = STATE_IDLE;
}

/*
 * goCusSettingState
 *
 * This function transitions the system to a custom setting state by performing a series of actions:
 * stopping the servo, providing buzzer feedback, clearing the LED, clearing the OLED display,
 * resetting the button status, and setting the system to the custom setting state.
 */
void goCusSettingState() {
	Led_Clear();
	statusoptton = 0;
	countTime = 0;
	customState = SETTING;
}

/*
 * goReadyState
 *
 * This function transitions the system to the "ready" state by performing the necessary actions:
 * playing a success buzzer sound, clearing the LED, clearing the OLED display, resetting the time counter,
 * resetting the button status, setting the pressing flag to true, and transitioning the system to the "close" state.
 */
void goReadyState() {
	Led_Clear();
	countTime = 0;
	statusoptton = 0;
	isPressing = true;
	currentState = STATE_CLOSE;
}

/*
 * goCloseState
 *
 * This function transitions the system to the "close" state. It performs the following actions:
 * - Clears the OLED display.
 * - Displays a "Closing..." message on the OLED.
 * - Plays a success buzzer sound.
 * - Activates the servo to close using the specified timer.
 * - Clears the LED indicators.
 * - Clears the OLED display again.
 * - Resets the time counter and button status.
 * - Sets the pressing flag to true.
 * - Keeps the system in the "close" state.
 *
 * Parameters:
 *   None
 */
void goCloseState() {

	Led_Clear();
	countTime = 0;
	statusoptton = 0;
	isPressing = true;
	currentState = STATE_CLOSE;
}

/*
 * goOpenState
 *
 * This function transitions the system to the "open" state. It performs the following actions:
 * - Clears the OLED display.
 * - Displays an "Opening..." message on the OLED.
 * - Plays a success buzzer sound.
 * - Activates the servo to open using the specified timer.
 * - Clears the LED indicators.
 * - Clears the OLED display again.
 * - Resets the time counter and button status.
 * - Sets the pressing flag to true.
 * - Sets the system state to "open".
 */
void goOpenState() {
	Led_Clear();
	countTime = 0;
	statusoptton = 0;
	isPressing = true;
	currentState = STATE_OPEN;
}

/*
 * goCustomState
 *
 * This function transitions the system to the "custom" state. It performs the following actions:
 * - Plays a success buzzer sound.
 * - Clears the LED indicators.
 * - Clears the OLED display.
 * - Resets the time counter and button status.
 * - Sets the pressing flag to true.
 * - Sets the system state to "custom" (configuration state).
 *
 * Parameters:
 *   None
 */
void goCustomState() {
	Led_Clear();
	countTime = 0;
	statusoptton = 0;
	isPressing = true;
	currentState = STATE_CONFIG;
}

/*
 * setDefaultPattern
 *
 * This function sets the default pattern for either opening or closing based on the input pattern type.
 * It updates the corresponding pattern array (`openPattern` or `closePattern`) and writes the pattern
 * using the `WriteOpenPattern` or `WriteClosePattern` function. The function also shows a "Saving..."
 * message on the OLED display and waits for 1 second before clearing the display.
 *
 * Parameters:
 *   - patrn: The pattern type to set, either `PATTERN_OPEN` or `PATTERN_CLOSE`.
 *     - `PATTERN_OPEN` sets the default open pattern.
 *     - `PATTERN_CLOSE` sets the default close pattern.
 */
void setDefaultPattern(PatternType patrn) {
	if (patrn == PATTERN_OPEN) {
		openPattern[0] = 1;
		openPattern[1] = 2;
		openPattern[2] = 3;
		WriteOpenPattern(openPattern);
	} else if (patrn == PATTERN_CLOSE) {
		closePattern[0] = 3;
		closePattern[1] = 2;
		closePattern[2] = 1;
		WriteClosePattern(closePattern);
	}
	HAL_Delay(1000);
}

/*
 * setDefaultPatternExitCustom
 *
 * This function sets the default pattern for either opening or closing based on the input pattern type
 * and exits the custom configuration. It updates the corresponding pattern array (`openPattern` or
 * `closePattern`) and writes the pattern using the `WriteOpenPattern` or `WriteClosePattern` function.
 * The function also clears the OLED display and waits for 1 second.
 *
 * Parameters:
 *   - patrn: The pattern type to set, either `PATTERN_OPEN` or `PATTERN_CLOSE`.
 *     - `PATTERN_OPEN` sets the default open pattern.
 *     - `PATTERN_CLOSE` sets the default close pattern.
 */
void setDefaultPatternExitCustom(PatternType patrn) {
	if (patrn == PATTERN_OPEN) {
		openPattern[0] = 1;
		openPattern[1] = 2;
		openPattern[2] = 3;
		WriteOpenPattern(openPattern);
	} else if (patrn == PATTERN_CLOSE) {
		closePattern[0] = 3;
		closePattern[1] = 2;
		closePattern[2] = 1;
		WriteClosePattern(closePattern);
	}

	Oled_Clear();
	HAL_Delay(1000);
}

/*
 * setCustomPattern
 *
 * This function allows the customization of the open or close pattern based on the specified pattern type.
 * The function displays a prompt on the OLED display and waits for button inputs to customize the pattern.
 * It ensures that valid pattern inputs are recorded and provides buzzer feedback. The function handles timeout
 * conditions for customizing the pattern and allows saving the pattern or returning to the previous settings.
 *
 * Parameters:
 *   - patrn: The pattern type to customize, either `PATTERN_OPEN` or `PATTERN_CLOSE`.
 *     - `PATTERN_OPEN` allows the user to customize the open pattern.
 *     - `PATTERN_CLOSE` allows the user to customize the close pattern.
 */
void setCustomPattern(PatternType patrn) {
	bool isProcess = true;
	uint32_t cnt = 0;
	uint32_t *pPattern[3];

	strcpy(msg, ">");
	if (patrn == PATTERN_OPEN) {
		pPattern[0] = &openPattern[0];
		pPattern[1] = &openPattern[1];
		pPattern[2] = &openPattern[2];

	} else if (patrn == PATTERN_CLOSE) {
		pPattern[0] = &closePattern[0];
		pPattern[1] = &closePattern[1];
		pPattern[2] = &closePattern[2];
	}

	*pPattern[0] = 0;
	*pPattern[1] = 0;
	*pPattern[2] = 0;

	Led_Show(true, true, true);

	while (isProcess) {
		if (ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {
			isPressing2 = true;
			if (pressStartTime == 0) {
				pressStartTime = HAL_GetTick();
				countTime = HAL_GetTick();
			} else if ((HAL_GetTick() - pressStartTime) >= 3000) {
				setDefaultPatternExitCustom(patrn);
				goCusSettingState();
				return;
			} else if ((HAL_GetTick() - pressStartTime) < 3000) {
				counttime3s = ((USER_TIMEOUT_3S - (HAL_GetTick() - countTime) + 999) / 1000);
				sprintf(msg2, "(SETTING in %u..)        ", counttime3s);
			}
		} else {
			if (isPressing2) {
				isPressing2 = false;
			} else {
				if (ReadOpttonValue(1) && !ReadOpttonValue(2) && !ReadOpttonValue(3)) {
					HAL_Delay(500);
					if (ReadOpttonValue(1) && !ReadOpttonValue(2) && !ReadOpttonValue(3)) {
						if (*pPattern[cnt - 1] != 1) {
							*pPattern[cnt] = 1;
							strcat(msg, " 1");

							Led_Show(false, true, true);
							cnt++;
						} else {

						}
					}
				}

				if (!ReadOpttonValue(1) && ReadOpttonValue(2) && !ReadOpttonValue(3)) {
					HAL_Delay(500);
					if (!ReadOpttonValue(1) && ReadOpttonValue(2) && !ReadOpttonValue(3)) {
						if (*pPattern[cnt - 1] != 2) {
							*pPattern[cnt] = 2;
							strcat(msg, " 2");

							Led_Show(true, false, true);
							cnt++;
						} else {

						}
					}
				}

				if (!ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {
					HAL_Delay(500);
					if (!ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {
						if (*pPattern[cnt - 1] != 3) {
							*pPattern[cnt] = 3;
							strcat(msg, " 3");

							Led_Show(true, true, false);
							cnt++;
						} else {

						}
					}
				}
				if (patrn == PATTERN_OPEN) {
					customState = CLOSE_BEGIN;
				} else {
					customState = DONE;
				}
				pressStartTime = 0;
			}
		}
		if (*pPattern[0] != 0 && *pPattern[1] != 0 && *pPattern[2] != 0) {
			if (patrn == PATTERN_OPEN && closePattern[0] == *pPattern[0] && closePattern[1] == *pPattern[1] && closePattern[1] == *pPattern[1]) {
				Led_Show(false, false, false);
				HAL_Delay(500);
				if (!ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {
					*pPattern[0] = 0;
					*pPattern[1] = 0;
					*pPattern[2] = 0;
					cnt = 0;

					strcpy(msg, ">");

				} else if ((ReadOpttonValue(1) || ReadOpttonValue(2)) && !ReadOpttonValue(3)) {

				}
			} else if (patrn == PATTERN_CLOSE && openPattern[0] == *pPattern[0] && openPattern[1] == *pPattern[1] && openPattern[1] == *pPattern[1]) {
				Oled_ShowMessage_sm("same Open pattern", 0, 35);
				Oled_ShowMessage_sm("Please Again(3)", 0, 53);
				Led_Show(false, false, false);
				HAL_Delay(500);
				if (!ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {
					*pPattern[0] = 0;
					*pPattern[1] = 0;
					*pPattern[2] = 0;
					cnt = 0;

					strcpy(msg, ">");
					Oled_ShowMessage_sm("                    ", 0, 23);
					Oled_ShowMessage_sm("                    ", 0, 35);
					Oled_ShowMessage_sm("                    ", 0, 53);
				} else if ((ReadOpttonValue(1) || ReadOpttonValue(2)) && !ReadOpttonValue(3)) {

				}
			} else {
				Oled_ShowMessage_sm("Ok(1)     Again(3)", 0, 53);
				Led_Show(false, false, false);

				if (ReadOpttonValue(1) && !ReadOpttonValue(2) && !ReadOpttonValue(3)) {
					HAL_Delay(500);
					if (ReadOpttonValue(1) && !ReadOpttonValue(2) && !ReadOpttonValue(3)) {
						Oled_Clear();
						Oled_ShowMessage_sm("Saving...", 0, 0);
						HAL_Delay(1000);
						isProcess = false;
					}
				} else if (!ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {
					HAL_Delay(500);
					if (!ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {
						*pPattern[0] = 0;
						*pPattern[1] = 0;
						*pPattern[2] = 0;
						cnt = 0;
						Led_Show(true, true, true);
					}
				} else if (!ReadOpttonValue(1) && ReadOpttonValue(2) && !ReadOpttonValue(3)) {

				}
			}
		}
		// Stored to flash memory
		if (patrn == PATTERN_OPEN) {
			WriteOpenPattern(*pPattern);
		} else if (patrn == PATTERN_CLOSE) {
			WriteClosePattern(*pPattern);
		}
	}
}

/*
 * showLedSensorIndex
 *
 * This function controls the LED display based on the specified sensor index and pattern type.
 * It retrieves the appropriate sensor pattern (either open or close) and displays the corresponding LED color.
 * The function supports up to 3 sensor indices, where each index is associated with a specific LED color:
 * - 1: Red LED
 * - 2: Green LED
 * - 3: Blue LED
 * If the index is invalid (greater than 2), the LED is turned off.
 *
 * Parameters:
 *   - index: The sensor index (0, 1, or 2) whose LED pattern will be shown.
 *   - patrn: The pattern type to display, either `PATTERN_OPEN` or `PATTERN_CLOSE`.
 *     - `PATTERN_OPEN` uses the openPattern to determine the LED color.
 *     - `PATTERN_CLOSE` uses the closePattern to determine the LED color.
 */
void showLedSensorIndex(uint32_t index, PatternType patrn) {
	if (index > 2) {
		Led_Show(false, false, false);
		return;
	}

	uint32_t targetSensor = openPattern[index];

	if (patrn == PATTERN_CLOSE) {
		targetSensor = closePattern[index];
	}

	switch (targetSensor) {
	case 1:
		Led_Show(true, false, false);

		break;
	case 2:
		Led_Show(false, true, false);

		break;
	case 3:
		Led_Show(false, false, true);

		break;
	default:
		Led_Show(false, false, false);
	}
}

/*
 * readSensorIndex
 *
 * This function checks the status of a sensor at a given index based on the specified pattern type.
 * It compares the button values to determine if the correct button is pressed for the given sensor.
 * The function supports up to 3 sensor indices and checks the corresponding button input for the sensor.
 * It returns `true` if the correct button is pressed for the selected sensor; otherwise, it returns `false`.
 * A small delay is added to debounce the button presses.
 *
 * Parameters:
 *   - index: The sensor index (0, 1, or 2) to be checked.
 *   - patrn: The pattern type to check, either `PATTERN_OPEN` or `PATTERN_CLOSE`.
 *     - `PATTERN_OPEN` checks the openPattern.
 *     - `PATTERN_CLOSE` checks the closePattern.
 *
 * Returns:
 *   - `true` if the correct button is pressed for the selected sensor.
 *   - `false` if the wrong button is pressed or the index is invalid.
 */
bool readSensorIndex(uint32_t index, PatternType patrn) {
	if (index > 2) {
		return false;
	}

	uint32_t targetSensor = openPattern[index];

	if (patrn == PATTERN_CLOSE) {
		targetSensor = closePattern[index];
	}

	switch (targetSensor) {
	case 1:
		if (ReadOpttonValue(1) && !ReadOpttonValue(2) && !ReadOpttonValue(3)) {
			HAL_Delay(500);
			if (ReadOpttonValue(1) && !ReadOpttonValue(2) && !ReadOpttonValue(3)) {
				return true;
			}
		}

		break;
	case 2:
		if (!ReadOpttonValue(1) && ReadOpttonValue(2) && !ReadOpttonValue(3)) {
			HAL_Delay(500);
			if (!ReadOpttonValue(1) && ReadOpttonValue(2) && !ReadOpttonValue(3)) {
				return true;
			}
		}

		break;
	case 3:
		if (!ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {
			HAL_Delay(500);
			if (!ReadOpttonValue(1) && !ReadOpttonValue(2) && ReadOpttonValue(3)) {
				return true;
			}
		}

		break;
	default:
		return false;
	}

	return false;
}
