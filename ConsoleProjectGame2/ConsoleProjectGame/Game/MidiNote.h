#pragma once
#include <vector>
#include <string>
#include "../ConsoleProjectGame/MidiParser/MidiFile.h"

struct NoteEvent 
{
    double timestamp; // �߻� �ð� (�� ����)
    int noteNumber;   // ��Ʈ ��ȣ
    int velocity;     // �ӵ� (0~127)
};

class MidiNote
{
public:

    inline std::vector<NoteEvent> parseMidiFile(const std::string& filePath)
    {
        smf::MidiFile midiFile;
        midiFile.read(filePath);
        midiFile.doTimeAnalysis(); // �ð� �м� (�� ����)

        std::vector<NoteEvent> notes;

        for (int track = 0; track < midiFile.getTrackCount(); ++track) {
            for (int event = 0; event < midiFile[track].size(); ++event) {
                smf::MidiEvent midiEvent = midiFile[track][event];

                if (midiEvent.isNoteOn()) {
                    double timestamp = midiEvent.seconds; // �� ���� �ð�
                    int noteNumber = midiEvent.getKeyNumber(); // ��Ʈ ��ȣ
                    int velocity = midiEvent.getVelocity(); // �ӵ�

                    notes.push_back({ timestamp, noteNumber, velocity });
                }
            }
        }

        return notes;
    }
};