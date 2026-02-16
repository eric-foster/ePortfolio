<center>
  <img src="./Images/profile.jpg" height=200 width=200>
</center>
  
# Computer Science Capstone
  
## <center>CS-499 | SNHU</center>

## Professional Self-Assessment

My experience in the Computer Science program at Southern New Hampshire University has played a critical role in shaping my professional goals, technical identity, and long-term career direction. Through progressive coursework and the development of a comprehensive ePortfolio, I have strengthened my ability to design, implement, analyze, and refine software systems using industry-aligned practices. This portfolio demonstrates my readiness to enter the computing systems domain, with a particular interest in backend systems, data-intensive applications, and performance-conscious software design. While no single portfolio can capture the full scope of my academic work, this collection highlights my strengths across three core areas: Software Design and Engineering, Algorithms and Data Structures, and Databases.

The artifacts presented in this ePortfolio reflect a cohesive progression toward building reliable, scalable, and maintainable systems. My Software Design and Engineering artifact demonstrates modular architecture, separation of concerns, and testable design. The Algorithms and Data Structures artifact showcases performance-driven decision-making through the use of self-balancing trees and empirical benchmarking. The Databases artifact emphasizes secure, efficient data access using a layered MongoDB CRUD architecture integrated with a Dash-based analytics dashboard. Together, these artifacts illustrate my ability to reason about systems holistically, considering data flow, performance, extensibility, and user interaction, while adhering to professional coding standards.

Beyond the individual artifacts, my coursework has strengthened my ability to collaborate in team environments, communicate technical ideas to diverse stakeholders, and operate within the full Software Development Life Cycle (SDLC). Courses involving group-based development required me to participate in code reviews, version control workflows, and structured documentation. These experiences taught me how to evaluate existing codebases, identify improvement opportunities, and justify design decisions using technical evidence. I have also gained experience translating requirements into technical implementations. Collectively, these experiences have reinforced my belief that high-quality software emerges from disciplined design, clear communication, and continuous refinement.

<code>Course Outcome 1</code>: I employed strategies for building collaborative environments that enable diverse audiences to support organizational decision-making in the field of computer science by conducting formal code reviews prior to implementing enhancements. Each enhancement cycle began with an analysis of existing functionality, identification of architectural limitations, and documentation of proposed improvements. This approach mirrors professional development workflows and ensures that technical decisions are traceable and defensible. In my Databases artifact, for example, the initial review identified scalability and security concerns, which informed subsequent enhancements such as environment-based credentials, indexed queries, and aggregation-based analytics.

<code>Course Outcome 2</code>: I designed, developed, and delivered professional-quality oral, written, and visual communications by producing detailed READMEs, enhancement narratives, and in-code documentation across all artifacts. These materials explain system architecture, design rationale, and usage instructions in a format appropriate for both technical and non-technical audiences. The clarity of this documentation demonstrates my ability to communicate complex technical systems in a structured and accessible manner.

<code>Course Outcome 3</code>: I designed and evaluated computing solutions using algorithmic principles and computer science practices by enhancing a modular C++ advising system that supports both Binary Search Tree (BST) and AVL Tree implementations. By replacing an unbalanced tree with a self-balancing AVL tree and adding benchmarking functionality, I demonstrated how algorithmic choices directly impact performance. This artifact reflects my ability to analyze time complexity, validate design decisions empirically, and select data structures appropriate to system constraints, which are skills that are essential in computing systems and backend engineering.

<code>Course Outcome 4</code>: I demonstrated the use of well-founded and innovative techniques, skills, and tools by developing modular, testable systems that integrate multiple technologies. Across the portfolio, I implemented layered architectures, common interfaces, server-side analytics, and database indexing strategies. These techniques improve scalability, maintainability, and performance while aligning with modern industry practices. My Databases artifact, in particular, demonstrates how backend services, data storage, and analytics pipelines can be composed into a cohesive system.

<code>Course Outcome 5</code>: I developed a security mindset that anticipates adversarial exploits by incorporating secure credential handling, controlled database access, and input-safe query construction. Credentials are loaded from environment variables rather than being hardcoded, database roles are explicitly defined, and MongoDB queries are structured to avoid injection vulnerabilities. These practices demonstrate my understanding that security is a foundational aspect of system design rather than an afterthought.

Collectively, the artifacts in this portfolio demonstrate my ability to design and reason about computing systems that are modular, efficient, secure, and maintainable. They reflect a consistent focus on backend logic, data management, and performance-aware engineering, all areas that align directly with my goal of pursuing a career in computing systems or systems-oriented software engineering. This ePortfolio serves not only as a record of my academic growth but also as a professional foundation for entering the software engineering field with a strong emphasis on building reliable and scalable systems.

### CODE REVIEW

A code review is a systematic quality assurance practice in which a project’s source code and resulting behavior are carefully examined to verify correctness, readability, and adherence to established standards. This process is essential to the success of computer science professionals because it promotes consistency across a codebase, identifies defects early, and reinforces high-quality coding practices throughout the software development lifecycle.

<code>You can watch the code review <a href="https://www.youtube.com/watch?v=w5S3wnJQVzc">here</a>.</code>

### Project 1: Software Engineering and Design

The artifact selected for this enhancement is an embedded thermostat control system originally developed during my Embedded Systems coursework. The project was initially created to demonstrate basic hardware interaction using a Raspberry Pi, including temperature sensing, LED indicators, button inputs, and LCD output. The original version implemented core functionality in a single script with direct hardware access. This artifact was created in the later stages of my undergraduate Computer Science program and served as a foundation for demonstrating embedded software concepts.

I selected this artifact for my ePortfolio because it strongly represents my growth in software design and engineering, particularly within computing systems and embedded software contexts. While the original implementation was functional, enhancing this artifact allowed me to refactor the system using professional software engineering principles. Specifically, I introduced a layered architecture, implemented a Hardware Abstraction Layer (HAL), separated configuration and control logic, and improved testability through the use of a simulated hardware interface. These improvements demonstrate my ability to design maintainable, scalable, and testable systems rather than focusing solely on functionality.

The enhanced artifact showcases key skills such as modular design, dependency inversion, interface-driven development, and clean separation of concerns. By abstracting hardware-specific logic away from the controller, the system can now be tested and extended without requiring physical hardware, reflecting industry-standard embedded development practices.

This enhancement primarily supports the course outcome focused on demonstrating the ability to use well-founded and innovative techniques, skills, and tools in computing practices to implement solutions that deliver value and accomplish industry-specific goals. Through the use of finite state machines, hardware abstraction, and automated testing, I demonstrated substantial progress toward this outcome.
Additionally, the enhancement contributes to the outcome related to designing and evaluating computing solutions using appropriate computer science practices and standards. The architectural refactoring required evaluating trade-offs between simplicity and maintainability, particularly when introducing additional abstraction layers.

Enhancing this artifact was a valuable learning experience that reinforced the importance of designing systems with long-term maintainability in mind. One of the most significant lessons I learned was how early architectural decisions can impact testability and extensibility. Introducing a Hardware Abstraction Layer required careful planning to ensure that the controller logic remained hardware-agnostic while still supporting all required functionality.
A primary challenge during this enhancement was adapting embedded code that was originally written with direct hardware access into a more modular and testable design. This process required me to rethink how hardware interactions were modeled and to create simulated components that accurately reflected real-world behavior. Overcoming these challenges strengthened my understanding of embedded system architecture and reinforced best practices used in professional software engineering environments.

Overall, this enhancement deepened my appreciation for structured design, testing strategies, and abstraction in computing systems. The improved artifact now more accurately represents my current skill set and aligns closely with my academic and professional goals within computing systems and embedded software engineering.

<center>
  <a href="" title="Click me to view the artifact report">
    <img src="" height=250>
  </a>
</center>
  
<code>See the artifact's report and code <a href="">here</a>.</code>

### Project 2: Algorithms and Data Structures

For my Algorithms and Data Structures enhancement, I selected my original CS300 final project, which implemented a basic academic advising tool using a Binary Search Tree (BST) for storing and searching course data. While the original version demonstrated functional correctness, its design tightly coupled data loading, tree logic, and user interface behavior within a single codebase, limiting extensibility and making it difficult to evaluate alternative data structures. To strengthen this artifact, I refactored the system into a modular architecture and introduced an enhanced version of the application that supports both a BST and an AVL tree implementation. This new structure allows the same course dataset to be loaded into either tree type while preserving identical user-facing behavior, enabling meaningful performance comparison and improving overall maintainability.

A major focus of the enhancement was implementing a benchmarking system that measures and compares search performance between the BST and AVL tree. The enhanced tool executes repeated search operations across all course IDs and records total search time and average time per operation, providing empirical data rather than theoretical assumptions. This addition demonstrates my ability to design controlled experiments within software, interpret performance results, and justify data structure choices based on measured efficiency. By integrating both tree types behind a consistent interface, I ensured that future data structures could be added with minimal changes to the rest of the system.

This enhancement demonstrates growth in my understanding of algorithmic tradeoffs, abstraction, and performance engineering. I moved beyond simply implementing a data structure toward designing a system that evaluates and validates its behavior under load. The modular architecture, comparative benchmarking, and clean separation of responsibilities collectively illustrate stronger software engineering practices and a deeper mastery of data structures. This artifact now represents not only functional correctness, but also thoughtful design, scalability, and analytical reasoning, all of which are skills that are essential for backend and systems-oriented software engineering roles.

<center>
  <a href="" title="Click me to view the artifact report">
    <img src="" height=400>
  </a>
</center>
  
<code>See the artifact's report and code <a href="">here</a>.</code>

### Project 3: Databases

The selected artifact for the databases category is a Python-based MongoDB CRUD module paired with an interactive Dash dashboard application developed originally in CS-340. The artifact consists of two primary components:

1. A reusable Animal_Shelter CRUD class that interfaces with a MongoDB database containing animal shelter records.
2. A Dash web dashboard that allows users to filter, view, and analyze these records through tables, charts, and an interactive map.

The original version of this artifact demonstrated foundational database connectivity and basic querying, but relied on hardcoded credentials, unbounded data retrieval, duplicated query logic, and client-side analytics. In this enhancement, the artifact was refactored to incorporate environment-based configuration, improved query efficiency, compound indexing, projection and pagination support, and server-side aggregation for analytics. These improvements elevate the artifact from a classroom project into a more scalable, secure, and production-oriented database application.

I selected this artifact because it clearly demonstrates my ability to design and implement database-driven software systems that go beyond simple CRUD functionality. The enhanced version showcases practical database engineering concepts such as secure configuration management, performance optimization, indexing strategies, and separation of concerns between data access and presentation layers.

Several components of this artifact highlight my database and software engineering skills:

- A modular CRUD class that supports projection, sorting, limiting, skipping, and aggregation
- Secure handling of credentials using environment variables instead of hardcoded values
- Compound indexes aligned with application query patterns
- Server-side aggregation pipelines for analytics rather than client-side computation
- Integration between MongoDB, Pandas, and Dash in a cohesive application architecture

These improvements demonstrate my ability to design database solutions that are maintainable, efficient, and aligned with industry practices. Compared to the original artifact, the enhanced version is more scalable, more secure, and better structured for real-world deployment.

This enhancement demonstrates progress toward multiple Computer Science program outcomes:

- Design and evaluate computing solutions using algorithmic principles and standards by optimizing database queries through projection, indexing, and aggregation pipelines.
- Use well-founded and innovative techniques, skills, and tools by leveraging MongoDB indexing, aggregation frameworks, and modular Python design.
- Develop a security mindset by eliminating hardcoded credentials and adopting environment-based configuration.
- Design and deliver professional-quality technical artifacts through improved code organization, documentation, and a production-style project structure.

Enhancing this artifact deepened my understanding of how database design choices directly affect application performance and scalability. I learned how inefficient patterns such as unbounded reads and client-side analytics can become serious bottlenecks as datasets grow. Implementing projection and server-side aggregation helped me see how MongoDB can offload expensive computation from the application layer.

One challenge was correctly integrating pagination and aggregation while preserving existing dashboard behavior. This required careful coordination between the CRUD module and Dash callbacks. Another challenge involved designing compound indexes that matched the most common query patterns rather than indexing individual fields blindly.

Overall, this enhancement strengthened my confidence in building database-centric applications that follow industry best practices. It reinforced the importance of secure configuration, thoughtful indexing, and modular design when working with real datasets and user-facing dashboards. Additionally, I gained a deeper appreciation for the trade-offs involved in database indexing. While indexes significantly improve query performance by reducing search time, they also introduce storage overhead and increase the cost of write operations such as inserts, updates, and deletes. To balance these trade-offs, I focused on creating compound indexes only on fields that align with the application’s most common query patterns, rather than indexing every field indiscriminately. This approach ensures faster reads where they matter most while minimizing unnecessary index maintenance, resulting in a more efficient and scalable system.
<center>
  <a href="">
    <img src="" height=400>
  </a>
</center>

  <code>See the artifact's report and code <b><a href="" title="Click me to view the artifact report">here</a></b>.</code>
