using UnityEngine;
using System.Collections;

public class UIManagerScript : MonoBehaviour {
	public void StartGame() {
		Application.LoadLevel ("DiveUnityDemo");
	}

	public void StartSettings() {
		Application.LoadLevel ("Settings");
	}

	public void StartMenu() {
		Application.LoadLevel ("MenuScene");
	}
}
