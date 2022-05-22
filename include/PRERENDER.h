static const unsigned char PUSH_EVERYTING[] =
                                {
                                    0x53,       // push rbx
                                    0x55,       // push rbp
                                    0x41, 0x54, // push r12
                                    0x41, 0x55, // push r13
                                    0x41, 0x56, // push r14
                                    0x41, 0x57  // push r15
                                };

static const unsigned char POP_EVERYTING[] =
                                {
                                    0x41, 0x5F, // push r15
                                    0x41, 0x5E, // push r14
                                    0x41, 0x5D, // push r13
                                    0x41, 0x5C, // push r12
                                    0x5D,       // push rbp
                                    0x5B        // push rbx
                                };
