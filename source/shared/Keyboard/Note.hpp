class ActiveNote {
    private:
        int m_originalPitch;
        int m_processedPitch;
        int m_velocity;

    public:
        ActiveNote(int originalPitch, int processedPitch, int velocity);
        int originalPitch() const;
        int pitch() const;
        int velocity() const;
};
