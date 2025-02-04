#pragma once
#include <vector>
#include <string>
#include "../ConsoleProjectGame/MidiParser/MidiFile.h"

struct NoteEvent 
{
    double timestamp; // 발생 시간 (초 단위)
    int noteNumber;   // 노트 번호
    int velocity;     // 속도 (0~127)
};

class MidiNote
{
public:

    inline std::vector<NoteEvent> parseMidiFile(const std::string& filePath)
    {
        smf::MidiFile midiFile;
        midiFile.read(filePath);
        midiFile.doTimeAnalysis(); // 시간 분석 (초 단위)

        std::vector<NoteEvent> notes;

        for (int track = 0; track < midiFile.getTrackCount(); ++track) {
            for (int event = 0; event < midiFile[track].size(); ++event) {
                smf::MidiEvent midiEvent = midiFile[track][event];

                if (midiEvent.isNoteOn()) {
                    double timestamp = midiEvent.seconds; // 초 단위 시간
                    int noteNumber = midiEvent.getKeyNumber(); // 노트 번호
                    int velocity = midiEvent.getVelocity(); // 속도

                    notes.push_back({ timestamp, noteNumber, velocity });
                }
            }
        }

        return notes;
    }
};